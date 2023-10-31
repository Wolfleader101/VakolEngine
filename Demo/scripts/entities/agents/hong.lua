local nav = nil;
local emotions = nil;
local atk = nil;

function init()
    entity:add_model("assets/models/ai/hong/hong.fbx", 1);

    local trans = entity:get_transform();
    trans.pos = Vector3.new(-10, 3, -10);

    trans.scale = Vector3.new(0.015, 0.015, 0.015);

    entity:add_script("emotions", "components/emotion.lua");
    emotions = entity:get_script("emotions");
    emotions.set_emotion(emotions.ANGER, 2);

    local rb = entity:add_rigid();
    rb.rot_lock = BVector3.new(true, false, true);
    rb.type = BodyType.Dynamic;
    entity:add_box_collider(Vector3.new(0.9, 1.75, 0.3));

    entity:add_script("navigation", "components/navigation.lua");
    nav = entity:get_script("navigation");

    entity:add_script("attack", "components/attack.lua");
    atk = entity:get_script("attack");

    local target = scene:get_camera():get_pos();

    nav.set_target(target, false);
    nav.MAX_DISTANCE = 0.8;

    nav.MOVE_SPEED = 0.025;
    nav.ROTATE_SPEED = 2.5;
    nav.BRAKE_FORCE = 1.0;

    nav.set_state("wander");
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

local entityToAttack = nil;

local ATTACK_DIST <const> = 0.5;

function tick()
    if (entityToAttack ~= nil) then
        if (distance(entity:get_transform().pos, entityToAttack:get_transform().pos) < ATTACK_DIST) then
            atk.attack(angryEntity);
            entityToAttack = nil;
        else
            return
        end
    end
    -- target = scene:get_camera():get_pos();
    nav.set_state("wander");


    -- TODO might only want to check nearby bins every 10 ticks or so???
    local nearby_bins = get_nearby_bins(entity, 100);

    for name, bin in pairs(nearby_bins) do
        local prev_bin_contents = prev_nearby_bins[name];
        local contents = bin:get_script("bin_content");

        -- if prev bin is not nil, meaning it was a bin he's previously checked nearby, see if the count has gone up
        -- memory should only store whats near him, as that can be percived that he is watching someone put it in the bin
        if (prev_bin_contents ~= nil) then
            -- print("Hong has seen this bin before")
            -- print_err(prev_bin_contents .. " " .. contents.COUNT)

            -- if the count has gone up, then he has seen someone put something in the bin and get angry
            if (prev_bin_contents < contents.COUNT) then
                print("Hong saw someone put something in the bin")
                -- TODO make hong angry
                local emotions = entity:get_script("emotions");
                local angerVal = emotions.get_emotion(emotions.ANGER);
                emotions.set_emotion(emotions.ANGER, angerVal + 0.8);

                entityToAttack = scene.globals.emotional_entities[1];
                local prevDistance = 1000000;
                for i = 2, #scene.globals.emotional_entities do --find closest agent to beat;
                    tempEnt = scene.globals.emotional_entities[i];
                    local dist = distance(entity:get_transform().pos, tempEnt:get_transform().pos);

                    if (not (tempEnt == entity) and dist < prevDistance) then
                        entityToAttack = tempEnt;
                        prevDistance = dist;
                    end
                end

                nav.set_target(entityToAttack:get_transform().pos, false);
                nav.set_state("chase");
            end

            -- TODO if it has gone down, then he has seen someone take something out of the bin and get happier??
            if (prev_bin_contents > contents.COUNT) then
                print("Hong saw someone take something out of the bin")
                -- TODO make hong happy
                local emotions = entity:get_script("emotions");
                local joyVal = emotions.get_emotion(emotions.JOY);
                emotions.set_emotion(emotions.JOY, joyVal + 0.8);
            end
        end

        --! this wont delete previous bins that it doesnt see anymore tho which is the downside to this approach
        prev_nearby_bins[name] = contents.COUNT;
    end

    -- if holding an item, increment the held time
    if (held_item ~= nil) then
        held_time = held_time + 1;

        -- if held time is greater than 100, throw the item
        -- TODO might want to throw this at someone??
        if (held_time >= 90) then
            held_item:get_script("interactable").interact(entity); -- throw the entity

            local emotions = entity:get_script("emotions");
            local joyVal = emotions.get_emotion(emotions.JOY);
            emotions.set_emotion(emotions.JOY, joyVal + 0.8);
            held_item = nil;
            held_time = 0;
        end
    end
end

function on_contact(other_ent)
    -- if already holding an item, do nothing
    if (held_item ~= nil) then
        local emotions = entity:get_script("emotions");
        local surpiseVal = emotions.get_emotion(emotions.SURPRISE);
        emotions.set_emotion(emotions.SURPRISE, surpiseVal + 0.7);
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
