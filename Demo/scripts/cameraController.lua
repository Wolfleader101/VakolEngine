function init(scene, entity)

end

local speed = 10;
local sprintSpeed = 50;

function update(scene, entity)
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
        velocity = sprintSpeed * Time["delta_time"];
    else
        velocity = speed * Time["delta_time"];
    end

    local camera = scene:get_camera();
    local old_pos = camera:get_pos();
    local forward = camera:get_forward();
    local right = camera:get_right();

    local new_pos = {
        x = old_pos.x + (forward.x * dir.z + right.x * dir.x) * velocity,
        -- y = old_pos.y + (forward.y * dir.z + right.y * dir.x) * velocity,
        y = 0,
        z = old_pos.z + (forward.z * dir.z + right.z * dir.x) * velocity,
    }

    new_pos.y =  scene:get_terrain():get_height(new_pos.x , new_pos.z) + 10.0;
    camera:set_pos(new_pos.x, new_pos.y, new_pos.z);

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