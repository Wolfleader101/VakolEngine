function init()
    print("Initialising Camera");

    state.flying = false;

    local camera = scene:get_camera();
    local pos = camera:get_pos();
    scene.globals.player.pos = pos;
    
    entity:get_transform().rot = Vector3.new(0.0, 180.0, 0.0);

    state.model = entity:add_model("assets/models/player/fps/FPS_KIT.fbx", 1.0, true, true);
    entity:set_shader("coreAssets/shaders/animation.prog");

    local shader = state.model:get_shader();

    shader:set_vec3v("light.direction", Vector3.new(math.rad(0.0), math.rad(0.0), math.rad(-90.0)));

    shader:set_float("material.shininess", 32.0);
    shader:set_vec3v("tint", Vector3.new(0.2, 0.65, 0.9));

    shader:set_int("material.diffuse_map", 0);
    shader:set_int("material.specular_map", 1);
    shader:set_int("material.normal_map", 2);
    shader:set_int("material.emission_map", 3);

    TIMER = 0.0;
end

function update()
    local moving = false;

    local dir = { x = 0, y = 0, z = 0 };
    local velocity = 0;

    if (Input:get_key(KEYS["KEY_W"])) then
        dir.z = 1;
        moving = true;
    end
    if (Input:get_key(KEYS["KEY_S"])) then
        dir.z = -1;
        moving = true;
    end
    if (Input:get_key(KEYS["KEY_A"])) then
        dir.x = -1;
        moving = true;
    end
    if (Input:get_key(KEYS["KEY_D"])) then
        dir.x = 1;
        moving = true;                    
    end
    if (Input:get_key(KEYS["KEY_LEFT_SHIFT"])) then
        scene.globals.player.is_sprinting = true;
    else
        scene.globals.player.is_sprinting = false;
    end

    velocity = scene.globals.player.curr_speed * Time.delta_time;

    if (Input:get_key_down(KEYS["KEY_Z"])) then
        state.flying = not state.flying
    end

    if (Input:get_mouse(KEYS["MOUSE_0"]) and wait(entity:get_animation_duration(1))) then
        entity:set_animation_state(1);
    elseif Input:get_mouse_up(KEYS["MOUSE_0"]) then
        entity:set_animation_state(0)
        entity:reset_animation(1);
        reset_timer();
    elseif not moving then
        entity:set_animation_state(0)
    elseif moving and not scene.globals.player.is_sprinting then
        entity:set_animation_state(11)
    elseif moving and scene.globals.player.is_sprinting then
        entity:set_animation_state(12)
    end

    local camera = scene:get_camera();
    local old_pos = camera:get_pos();
    local forward = camera:get_forward();
    local right = camera:get_right();

    local new_pos = Vector3.new(
        old_pos.x + (forward.x * dir.z + right.x * dir.x) * velocity,
        old_pos.y + (forward.y * dir.z + right.y * dir.x) * velocity,
        old_pos.z + (forward.z * dir.z + right.z * dir.x) * velocity
    )

    if (not state.flying) then
        local terr_scale = scene.globals.terrain.transform.scale;
        new_pos.y = (scene.globals.terrain.terr:get_height(new_pos.x / terr_scale.x, new_pos.z / terr_scale.z) * terr_scale.y) + 0.5;
    end

    entity:get_transform().pos = Vector3.new(new_pos.x, new_pos.y - 0.70, new_pos.z);

    camera:set_pos(new_pos.x, new_pos.y, new_pos.z);
    scene.globals.player.pos = new_pos;

    local targetRotation = math.atan(forward.x, forward.z)
    entity:get_transform().rot.y = math.deg(targetRotation)

    local delta_mouse_pos = Input:get_delta_mouse_pos();
    camera:set_yaw(camera:get_yaw() + delta_mouse_pos.x * 0.05);

    local pitch = camera:get_pitch() + delta_mouse_pos.y * 0.05;
    if (pitch > 89.0) then
        pitch = 89.0;
    elseif (pitch < -89.0) then
        pitch = -89.0;
    end

    camera:set_pitch(pitch);
end

function wait(seconds)
    TIMER = TIMER + Time.delta_time;

    if (TIMER >= seconds) then
        return true;
    end

    return false;
end

function reset_timer()
    TIMER = 0.0;
end