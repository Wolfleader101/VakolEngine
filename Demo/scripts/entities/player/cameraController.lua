function init()
    print("Initialising Camera");

    state.speed = 2;
    state.sprintSpeed = 3;

    local camera = scene:get_camera();
    local pos = camera:get_pos();
    scene.globals.player = {pos = pos}
end

function update()
    local dir = { x = 0, y = 0, z = 0 };
    local velocity = 0;

    if (Input:get_key(KEYS["KEY_W"])) then
        dir.z = 1;
    end
    if (Input:get_key(KEYS["KEY_S"])) then
        dir.z = -1;
    end
    if (Input:get_key(KEYS["KEY_A"])) then
        dir.x = -1;
    end
    if (Input:get_key(KEYS["KEY_D"])) then
        dir.x = 1;
    end
    if (Input:get_key(KEYS["KEY_LEFT_SHIFT"])) then
        velocity = state.sprintSpeed * Time.delta_time;
    else
        velocity = state.speed * Time.delta_time;
    end

    local camera = scene:get_camera();
    local old_pos = camera:get_pos();
    local forward = camera:get_forward();
    local right = camera:get_right();

    local new_pos = Vector3.new(
        old_pos.x + (forward.x * dir.z + right.x * dir.x) * velocity,
        0,
        old_pos.z + (forward.z * dir.z + right.z * dir.x) * velocity
    )

    local terr_entity = scene:get_entity("terrain");
    local terr_scale = terr_entity:get_transform().scale;
    new_pos.y = (terr_entity:get_terrain():get_height(new_pos.x / terr_scale.x, new_pos.z / terr_scale.z) * terr_scale.y) + 0.15;
    
    camera:set_pos(new_pos.x, new_pos.y, new_pos.z);
    scene.globals.player.pos = new_pos;


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