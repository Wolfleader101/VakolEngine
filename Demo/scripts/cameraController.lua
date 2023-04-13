function init(scene, entity)
    print("Hello from cameraController");
    camera = scene:get_camera();
    print("Camera Pos")
    print(camera:get_pos().x)
end

function update(scene, entity)
    local dir = { x = 0, y = 0, z = 0 };
    local velocity = 0;
    if (Input:get_key(KEYS["KEY_W"]))
    then
        dir.z = 1;
    end
    if (Input:get_key(KEYS["KEY_S"]))
    then
        dir.z = -1;
    end
    if (Input:get_key(KEYS["KEY_A"]))
    then
        dir.x = -1;
    end
    if (Input:get_key(KEYS["KEY_D"]))
    then
        dir.x = 1;
    end
    if (Input:get_key(KEYS["KEY_LEFT_SHIFT"]))
    then
        velocity = 50 * Time["delta_time"];
    else
        velocity = 30 * Time["delta_time"];
    end
    camera = scene:get_camera();
    old_pos = camera:get_pos();
    forward = camera:get_forward();
    right = camera:get_right();
    
    new_pos = {
        x = old_pos.x + (forward.x * dir.z + right.x * dir.x) * velocity,
        y = old_pos.y + (forward.y * dir.z + right.y * dir.x) * velocity,
        z = old_pos.z + (forward.z * dir.z + right.z * dir.x) * velocity}
    camera:set_pos(new_pos.x, new_pos.y, new_pos.z);


    delta_mouse_pos = Input:get_delta_mouse_pos();
    print(delta_mouse_pos.x)
    camera:set_yaw(camera:get_yaw() + delta_mouse_pos.x * 0.05);


    pitch = camera:get_pitch() + delta_mouse_pos.y * 0.05;
    if (pitch > 89.0)
    then
        pitch = 89.0;
    end
    if (pitch < -89.0)
    then
        pitch = -89.0;
    end

    camera:set_pitch(pitch);

end