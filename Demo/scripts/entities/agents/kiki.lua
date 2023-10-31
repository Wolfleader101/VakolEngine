local nav = nil;
local emotions = nil;

local body_mesh = nil;
local eye_mesh = nil;

local emotional_agents = nil;

function random_float(lower, greater)
    return lower + math.random() * (greater - lower);
end

function init()
    local model = entity:add_model("assets/models/ai/kiki/kiki.obj", 0.5);
        
    body_mesh = model:get_mesh(0);
    eye_mesh = model:get_mesh(1);

    body_mesh.material:use_colors_and_textures(true);
        
    body_mesh.material:set_light_type(LightType.Directional);
    body_mesh.material:set_light_direction(Vector3.new(0.0, 1.0, 0.0));

    eye_mesh.material:set_opacity(0.0);
    eye_mesh.material:use_lighting(false);

    entity:add_script("emotions", "components/emotion.lua");
    emotions = entity:get_script("emotions");

    local rb = entity:add_rigid();
    rb.rot_lock = BVector3.new(true, true, true);

    entity:add_box_collider(model:get_half_extents(0));

    entity:add_script("navigation", "components/navigation.lua");
    
    nav = entity:get_script("navigation");

    -- OFFSETS FOR KIKI EYE EXPRESSIONS
    -- (0.0, 0.0), (0.0, 1.0), (1.0, 0.0), (1.0, 1.0) = neutral
    -- (0.0, 0.5), (0.0, 1.5), (1.0, 0.5), (1.0, 1.5) = happy
    -- (0.5, 0.0), (0.5, 1.0), (1.5, 0.0), (1.5, 1.0) = angry
    -- (0.5, 0.5), (0.5, 1.5), (1.5, 0.5), (1.5, 1.5) = dizzy

    emotional_agents = scene.globals.emotional_entities;

    nav.MAX_DISTANCE = 1.0;
 
    nav.MOVE_SPEED = 0.05;
    nav.ROTATE_SPEED = 5.0;
    nav.BRAKE_FORCE = 1.0;

    nav.set_state("chase");
end

local prev_nearby_bins = {}; -- store {name: rubbishCount}

local held_item = nil;
local held_time = 0;

local function get_nearby_bins(origin_entity, trigger_distance)
    local origin_pos = origin_entity:get_transform().pos

    local nearby_bins = {}
    for name, bin in pairs(scene.globals.bins) do
        if bin ~= origin_entity then
            local diff = origin_pos - bin:get_transform().pos
            if diff:magnitude() <= trigger_distance then
                nearby_bins[name] = bin;
            end
        end
    end

    return nearby_bins
end

local function get_nearby_emotional_agents(origin_entity, trigger_distance)
    for i = 1, #emotional_agents do
        if (emotional_agents[i] ~= entity) then

            local entityPos = emotional_agents[i]:get_transform().pos;
            local distanceToEntity = distance(entityPos, entity:get_transform().pos)

            if (distanceToEntity < trigger_distance) then
                nav.TARGET = entityPos;

                local agentEmotionalState = emotional_agents[i]:get_script("components/emotions.lua");

                if (agentEmotionalState ~= nil) then
                    if (agentEmotionalState:get_emotion(emotions.JOY)) then
                        local angerValue = emotions.get_emotion(emotions.ANGER);
                        emotions.set_emotion(emotions.ANGER, angerValue + 0.6);
                    end
                end

                break;
            end
        end
    end
end

function tick()
    -- if holding an item, increment the held time
    if (held_item ~= nil) then
        held_time = held_time + 1;

        -- if held time is greater than 100, throw the item
        -- TODO might want to throw this at someone??
        if (held_time >= 90) then
            held_item:get_script("interactable").interact(entity); -- throw the entity

            local joyVal = emotions.get_emotion(emotions.JOY);
            emotions.set_emotion(emotions.JOY, joyVal + 0.6);
            held_item = nil;
            held_time = 0;
        end
    end

    -- TODO might only want to check nearby bins every 10 ticks or so???
    local nearby_bins = get_nearby_bins(entity, 100);
    get_nearby_emotional_agents(entity, 100);

    for name, bin in pairs(nearby_bins) do
        local prev_bin_contents = prev_nearby_bins[name];
        local contents = bin:get_script("bin_content");

        if (prev_bin_contents ~= nil) then

            -- if the count has gone up, then he has seen someone put something in the bin and get angry
            if (prev_bin_contents < contents.COUNT) then
                print("Kiki saw someone put something in the bin")
                local angerVal = emotions.get_emotion(emotions.ANGER);
                emotions.set_emotion(emotions.ANGER, angerVal + 0.9);
            end

            -- TODO if it has gone down, then he has seen someone take something out of the bin and get happier??
            if (prev_bin_contents > contents.COUNT) then
                print("Kiki saw someone take something out of the bin")

                local joyVal = emotions.get_emotion(emotions.JOY);
                emotions.set_emotion(emotions.JOY, joyVal + 0.8);
            end
        end

        --! this wont delete previous bins that it doesnt see anymore tho which is the downside to this approach
        prev_nearby_bins[name] = contents.COUNT;
    end

    local maxEmotion = emotions.JOY
    local maxEmotionValue = emotions.get_emotion(emotions.JOY)

    -- Check for other emotions and their values
    local emotionsToCheck = {emotions.ANTICIPATION, emotions.ANGER, emotions.DISGUST, emotions.SURPRISE}

    for _, emotion in ipairs(emotionsToCheck) do
        local emotionValue = emotions.get_emotion(emotion)
        if emotionValue > maxEmotionValue then
            maxEmotion = emotion
            maxEmotionValue = emotionValue
        end
    end

    -- Set the eye appearance based on the emotion with the highest value
    local eyeOffset = Vector2.new(0.0, 0.0)

    if (maxEmotion == emotions.ANTICIPATION) then
        eyeOffset = Vector2.new(0.0, 0.0);
    elseif (maxEmotion == emotions.JOY) then
        eyeOffset = Vector2.new(0.0, 0.5);
    elseif (maxEmotion == emotions.ANGER) then
        eyeOffset = Vector2.new(0.5, 0.0);
    elseif (maxEmotion == emotions.DISGUST or maxEmotion == emotions.SURPRISE) then
        eyeOffset = Vector2.new(0.5, 0.5);
    end

    -- Apply the eye appearance based on the calculated offset
    eye_mesh.material:set_uv_offset(eyeOffset)
end

function on_contact(other_ent)
    -- if already holding an item, do nothing
    if (held_item ~= nil) then
        local surpiseVal = emotions.get_emotion(emotions.SURPRISE);
        emotions.set_emotion(emotions.SURPRISE, surpiseVal + 0.2);

        return;
    end

    local affordance = other_ent:get_script("affordance");

    -- only if the item affords holding, pick it up
    -- TODO might also want to only pick up items that are holdable and throwable??
    if (affordance ~= nil and affordance.AFFORDANCES.HOLDING == 1.0) then
        print(other_ent:get_tag() .. " Affords Holding")
        held_item = other_ent;
        if (held_item:get_script("interactable") ~= nil) then
            held_item:get_script("interactable").interact(entity);
        end
    end
end