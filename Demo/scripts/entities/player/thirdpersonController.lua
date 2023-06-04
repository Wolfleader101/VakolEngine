function init()
    print("Initialising Third Person Player Controller");

    state.speed = 2;
    state.sprintSpeed = 4.25;
    state.flying = false;
    state.cameraDistance = {y = 1.25, z = 2}

    -- Load model and set initial animation
    state.model = entity:add_model("assets/models/enemy.glb", 0.003, true, true) -- get model and add a drawable component
    state.model:set_shader("coreAssets/shaders/animation.prog") -- set the shader on the model (automatically binds it)

    state.model:set_animation_state(1);

    local shader = state.model:get_shader(); -- get the shader from the model

    shader:set_vec3v("light.direction", Vector3.new(math.rad(0.0), math.rad(0.0), math.rad(-90.0))); -- point light straight up (makes him look evil)

    shader:set_float("material.shininess", 32.0);
    shader:set_vec3v("tint", Vector3.new(0.2, 0.65, 0.9));

    shader:set_int("material.diffuse_map", 0);
    shader:set_int("material.specular_map", 1);
    shader:set_int("material.normal_map", 2);
    shader:set_int("material.emission_map", 3);
end

function update()
    local dir = { x = 0, y = 0, z = 0 };
    local velocity = 0;
    local moving = false  -- Is the player moving?

    if (Input:get_key(KEYS["KEY_W"])) then
        dir.z = 1;
        moving = true
    end
    if (Input:get_key(KEYS["KEY_S"])) then
        dir.z = -1;
        moving = true
    end
    if (Input:get_key(KEYS["KEY_A"])) then
        dir.x = -1;
        moving = true
    end
    if (Input:get_key(KEYS["KEY_D"])) then
        dir.x = 1;
        moving = true
    end
    if (Input:get_key(KEYS["KEY_LEFT_SHIFT"])) then
        velocity = state.sprintSpeed * Time.delta_time;
    else
        velocity = state.speed * Time.delta_time;
    end

    if (Input:get_key_down(KEYS["KEY_Z"])) then
        state.flying = not state.flying
    end

    local camera = scene:get_camera();
    local old_pos = entity:get_transform().pos;
    local forward = camera:get_forward();
    local right = camera:get_right();

    local new_pos = Vector3.new(
        old_pos.x + (forward.x * dir.z + right.x * dir.x) * velocity,
        old_pos.y + (forward.y * dir.z + right.y * dir.x) * velocity,
        old_pos.z + (forward.z * dir.z + right.z * dir.x) * velocity
    )

    local terr_scale = scene.globals.terrain.transform.scale;

    if (not state.flying) then
        new_pos.y = (scene.globals.terrain.terr:get_height(new_pos.x / terr_scale.x, new_pos.z / terr_scale.z) * terr_scale.y) + 0.05;
    end
    
    entity:get_transform().pos = new_pos;
    local forwardFlat = Vector3.new(forward.x, 0, forward.z):normalize()
    
    local cameraPos = Vector3.new(
        new_pos.x - forwardFlat.x * state.cameraDistance.z,
        new_pos.y + state.cameraDistance.y,
        new_pos.z - forwardFlat.z * state.cameraDistance.z
    )

     local terrainHeight = scene.globals.terrain.terr:get_height(cameraPos.x / terr_scale.x, cameraPos.z / terr_scale.z) * terr_scale.y
    if cameraPos.y < terrainHeight then
        cameraPos.y = terrainHeight + 1.25
    end
    
    camera:set_pos(cameraPos.x, cameraPos.y, cameraPos.z)
    

    -- Rotate model to match the direction of movement (you may have to adjust this depending on your model)
    if moving then
        local targetRotation = math.atan(forward.x, forward.z)
        targetRotation = targetRotation * (180 / math.pi)
        entity:get_transform().rot.y = targetRotation
    end

    local delta_mouse_pos = Input:get_delta_mouse_pos();
    camera:set_yaw(camera:get_yaw() + delta_mouse_pos.x * 0.05);

    local pitch = camera:get_pitch() + delta_mouse_pos.y * 0.05;
    if (pitch > 89.0) then
        pitch = 89.0;
    elseif (pitch < -89.0) then
        pitch = -89.0;
    end

    camera:set_pitch(pitch);

    -- Play running animation if player is moving, idle animation otherwise
    if moving then
        state.model:set_animation_state(3)
    else
        state.model:set_animation_state(1)
    end

    -- Update animation
    state.model:update_animation(Time.delta_time)
end