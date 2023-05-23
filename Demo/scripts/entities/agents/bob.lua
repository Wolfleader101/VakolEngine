function init()
    local IDLE_STATE_DEFAULT = 0;
    local IDLE_STATE_SEARCH = 1;

    local IDLE_STATE_PLAYER_SPOTTED = 2;

    local RUN_STATE_REGULAR = 3;
    local RUN_STATE_ENRAGED = 4;

    local ATTACK_STATE_SLASH_LIGHT = 5;
    local ATTACK_STATE_SLASH_HEAVY = 6;

    local ATTACK_STATE_PUNCH_LIGHT = 7;
    local ATTACK_STATE_SLAM_HEAVY = 8; 

    TIMER = 0.0;
    
    entity:get_transform().pos = Vector3.new(2.7, 0, -7.0);

    state.model = entity:add_model("assets/models/enemy.glb", 0.003, true) -- get model and add a drawable component
    state.model:set_shader("coreAssets/shaders/animation.prog") -- set the shader on the model (automatically binds it)

    state.model:set_animation_state(IDLE_STATE_SEARCH);

    local shader = state.model:get_shader(); -- get the shader from the model

    --shader:set_vec3v("light.position", Vector3.new(2.0, 2.0, -4.0));
    shader:set_vec3v("light.direction", Vector3.new(math.rad(0.0), math.rad(0.0), math.rad(-90.0))); -- point light straight up (makes him look evil)

    shader:set_float("material.shininess", 32.0);
    shader:set_vec3v("tint", Vector3.new(0.2, 0.65, 0.9));

    shader:set_int("material.diffuse_map", 0);
    shader:set_int("material.specular_map", 1);
    shader:set_int("material.normal_map", 2);
    shader:set_int("material.emission_map", 3);

    state.fsm = entity:add_fsm();

    state.fsm:add_state("idle_search", function()
        state.model:set_animation_state(IDLE_STATE_SEARCH)

        local diff = scene.globals.player.pos - entity:get_transform().pos;
        local player_dist = diff:magnitude();

        if (player_dist < 7) then
            state.fsm:change_state("idle_player_spotted");
        end
    end)

    state.fsm:add_state("idle_player_spotted", function()
        state.model:set_animation_state(IDLE_STATE_PLAYER_SPOTTED);

        -- Calculate direction vector towards the player
        local direction = (scene.globals.player.pos - entity:get_transform().pos):normalize();

        -- Rotate to face the player
        local targetRotation = math.atan(direction.x, direction.z);  -- Compute target rotation (assuming Y-up coordinate system)
        targetRotation = targetRotation * (180 / math.pi);  -- Convert from radians to degrees
        entity:get_transform().rot.y = targetRotation;  -- Set entity's Y rotation to face the player (adjust this according to your coordinate system)

        if (wait(2.5)) then
            state.fsm:change_state("chase");
            TIMER = 0.0;
        end
    end)

    state.fsm:add_state("chase", function()
        state.model:set_animation_state(RUN_STATE_ENRAGED); 

        -- Calculate direction vector towards the player
        local direction = (scene.globals.player.pos - entity:get_transform().pos):normalize();

        -- Move towards the player
        local speed = 4;  -- Adjust this value as needed, can later be set as global variable based on difficulty
        local newPos = entity:get_transform().pos + direction * speed * Time.delta_time;
        entity:get_transform().pos.x = newPos.x;
        entity:get_transform().pos.z = newPos.z;

        -- Rotate to face the player
        local targetRotation = math.atan(direction.x, direction.z);  -- Compute target rotation (assuming Y-up coordinate system)
        targetRotation = targetRotation * (180 / math.pi);  -- Convert from radians to degrees
        entity:get_transform().rot.y = targetRotation;  -- Set entity's Y rotation to face the player (adjust this according to your coordinate system)

        -- If player is no longer within 10m radius, switch back to 'idle' state
        local diff = scene.globals.player.pos - entity:get_transform().pos;
        local player_dist = diff:magnitude();

        if player_dist > 7 then
            state.fsm:change_state("idle_search")
        end
        
        if player_dist < 0.75 then
            state.fsm:change_state("attack")
        end
    end)

    state.fsm:add_state("attack", function()
        -- Calculate direction vector towards the player
        local direction = (scene.globals.player.pos - entity:get_transform().pos):normalize();

        -- Rotate to face the player
        local targetRotation = math.atan(direction.x, direction.z);  -- Compute target rotation (assuming Y-up coordinate system)
        targetRotation = targetRotation * (180 / math.pi);  -- Convert from radians to degrees
        entity:get_transform().rot.y = targetRotation;  -- Set entity's Y rotation to face the player (adjust this according to your coordinate system)

        --random_anim = math.random(ATTACK_STATE_SLASH_LIGHT, ATTACK_STATE_SLAM_HEAVY);
        state.model:set_animation_state(ATTACK_STATE_SLASH_LIGHT);

        -- If player is no longer within 1m radius, switch back to 'chase' state
        local diff = scene.globals.player.pos - entity:get_transform().pos;
        local player_dist = diff:magnitude();

        if (player_dist > 0.65) then
            if (wait(0.5)) then
                state.fsm:change_state("chase");
                TIMER = 0.0;
            end
        end
    end)

    -- Set the initial state
    state.fsm:change_state("idle_search")

    print_err("Bob is ready")
    scene.globals.bobState = state.fsm;

end

function wait(seconds)
    TIMER = TIMER + Time.delta_time;

    if (TIMER >= seconds) then
        return true;
    end

    return false;
end

function update()
    state.fsm:update()
    local pos = entity:get_transform().pos;
    local terr_scale = scene.globals.terrain.transform.scale;
    pos.y = (scene.globals.terrain.terr:get_height(pos.x / terr_scale.x, pos.z / terr_scale.z) * terr_scale.y) + 0.05;
end