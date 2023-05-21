function init()
    local IDLE_STATE = 0;
    local WALK_STATE = 1;
    local RUN_STATE = 2;
    
    entity:get_transform().pos = Vector3.new(3.0, 0, -7.0);

    state.model = entity:add_model("assets/models/test.glb", 0.001, true) -- get model and add a drawable component
    state.model:set_shader("coreAssets/shaders/animation.prog") -- set the shader on the model (automatically binds it)

    state.model:set_animation_state(IDLE_STATE);

    local shader = state.model:get_shader(); -- get the shader from the model

    shader:set_vec3v("light.position", Vector3.new(2.0, 2.0, -4.0));
    -- shader:set_vec3v("light.direction", Vector3.new(math.rad(0.0), math.rad(0.0), math.rad(90.0)));

    shader:set_float("material.shininess", 16.0);
    shader:set_vec3v("tint", Vector3.new(0.2, 0.65, 0.9));

    shader:set_int("material.diffuse_map", 0);
    shader:set_int("material.specular_map", 1);
    shader:set_int("material.normal_map", 2);
    shader:set_int("material.emission_map", 3);



    state.fsm = entity:add_fsm();

    state.fsm:add_state("idle", function()
        -- print("Bob is idle")

        state.model:set_animation_state(0);

        local diff = scene.globals.player.pos - entity:get_transform().pos;
        local player_dist = diff:magnitude();

        if player_dist < 3 then
            state.fsm:change_state("attacking")
        end


        -- if(Input:get_key(KEYS["KEY_2"])) then
        --     state.fsm:change_state("moving")
        -- end
    end)
    
    state.fsm:add_state("moving", function()
        -- print("Bob is moving")
        state.model:set_animation_state(2);

        local pos = entity:get_transform().pos;
        pos.x = pos.x;
        pos.z = pos.z + 0.5 * Time.delta_time;

        if(Input:get_key(KEYS["KEY_1"])) then
            state.fsm:change_state("idle")
        end
    end)

    state.fsm:add_state("attacking", function()
        state.model:set_animation_state(2); 

        -- Calculate direction vector towards the player
        local direction = (scene.globals.player.pos - entity:get_transform().pos):normalize();

        -- Move towards the player
        local speed = 1;  -- Adjust this value as needed, can later be set as global variable based on difficulty
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
        if player_dist > 4 then
            state.fsm:change_state("idle")
        end
    end)

    -- Set the initial state
    state.fsm:change_state("idle")

    print_err("Bob is ready")
    scene.globals.bobState = state.fsm;

end


function update()
        local pos = entity:get_transform().pos;
        local terr_entity = scene:get_entity("terrain");
        local terr_scale = terr_entity:get_transform().scale;
        pos.y = (terr_entity:get_terrain():get_height(pos.x / terr_scale.x, pos.z / terr_scale.z) * terr_scale.y) + 0.01;
    state.fsm:update()
end