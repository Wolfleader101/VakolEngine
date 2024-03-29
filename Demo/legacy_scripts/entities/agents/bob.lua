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

    local MIN_ATTACK_DISTANCE = 0.7;
    local MAX_ATTACK_DISTANCE = 1.2;
    local RUN_DISTANCE = 2.5;
    local VIEW_DISTANCE = 10.0;

    local SPRINT_STATE = 1;

    local RUN_SPEED = 1.25;
    local SPRINT_SPEED = 3.0;

    TIMER = 0.0;
    PAUSE_ANIMATION = false;
    ONCE = false;
    
    entity:get_transform().pos = Vector3.new(2.7, 0, -12.0);

    state.model = entity:add_model("assets/models/agents/bob.glb", 0.003, true, true) -- get model and add a drawable component
    entity:set_shader("coreAssets/shaders/animation.prog") -- set the shader on the model (automatically binds it)

    state.model:set_animation_state(IDLE_STATE_SEARCH);

    local shader = state.model:get_shader(); -- get the shader from the model

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

        if (player_dist < VIEW_DISTANCE) then
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
        -- Calculate direction vector towards the player
        local direction = (scene.globals.player.pos - entity:get_transform().pos):normalize();

        -- Move towards the player
        local speed = SPRINT_STATE and SPRINT_SPEED or not SPRINT_STATE and RUN_SPEED;  -- Adjust this value as needed, can later be set as global variable based on difficulty

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

        if (player_dist < RUN_DISTANCE) then
            state.model:set_animation_state(RUN_STATE_REGULAR);
            SPRINT_STATE = 0;
        elseif (player_dist > RUN_DISTANCE) then
            state.model:set_animation_state(RUN_STATE_ENRAGED);
            SPRINT_STATE = 1;
        end

        if player_dist > VIEW_DISTANCE then
            state.fsm:change_state("idle_search")
        end
        
        if player_dist < MIN_ATTACK_DISTANCE then
            ONCE = false;
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

        local random_attack = ATTACK_STATE_SLASH_LIGHT;

        if (not ONCE) then
            random_attack = math.random(ATTACK_STATE_SLASH_LIGHT, ATTACK_STATE_SLAM_HEAVY);
            state.model:set_animation_state(random_attack);
            ONCE = not ONCE;
        end

        -- If player is no longer within 1m radius, switch back to 'chase' state
        local diff = scene.globals.player.pos - entity:get_transform().pos;
        local player_dist = diff:magnitude();

        if (player_dist > MAX_ATTACK_DISTANCE) then
            if (wait(state.model:get_animation_duration() * 0.15)) then
                state.model:reset_current_animation();
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

function tick()
    state.fsm:update()
    local pos = entity:get_transform().pos;
    local terr_scale = scene.globals.terrain.transform.scale;
    pos.y = (scene.globals.terrain.terr:get_height(pos.x / terr_scale.x, pos.z / terr_scale.z) * terr_scale.y) + 0.05;

    if (Input:get_key_down(KEYS["KEY_1"])) then
        state.model:reset_current_animation();
    end

    if (Input:get_key_down(KEYS["KEY_2"])) then
        PAUSE_ANIMATION = not PAUSE_ANIMATION;
    end
end