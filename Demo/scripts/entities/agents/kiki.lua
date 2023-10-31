local emotions = nil;

function random_float(lower, greater)
    return lower + math.random() * (greater - lower);
end

function init()
    local model = entity:add_model("assets/models/ai/kiki/kiki.obj", 0.5);
        
    local body_mesh = model:get_mesh(0);
    local eye_mesh = model:get_mesh(1);

    body_mesh.material:set_diffuse_color(Vector4.new(random_float(0.0, 1.0), random_float(0.0, 1.0), random_float(0.0, 1.0), 1.0));
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
    
    local nav = entity:get_script("navigation");

    -- OFFSETS FOR KIKI EYE EXPRESSIONS
    -- (0.0, 0.0), (0.0, 1.0), (1.0, 0.0), (1.0, 1.0) = neutral
    -- (0.0, 0.5), (0.0, 1.5), (1.0, 0.5), (1.0, 1.5) = happy
    -- (0.5, 0.0), (0.5, 1.0), (1.5, 0.0), (1.5, 1.0) = angry
    -- (0.5, 0.5), (0.5, 1.5), (1.5, 0.5), (1.5, 1.5) = dizzy

    eye_mesh.material:set_uv_offset(Vector2.new(0.5, 0.0));

    nav.TARGET = scene:get_camera():get_pos();

    nav.MAX_DISTANCE = 5.0;
 
    nav.MOVE_SPEED = 0.05;
    nav.ROTATE_SPEED = 5.0;
    nav.BRAKE_FORCE = 1.0;

    nav.set_state("wander");
end

local held_item = nil;
local held_time = 0;

function tick()
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