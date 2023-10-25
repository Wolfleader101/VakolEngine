local is_sprinting = false;

function init()
    entity:get_transform().rot = Vector3.new(0.0, 0.0, 0.0);

    scene.globals.player =
    {
        pos = Vector3.new(0.0, 0.0, 0.0)
    }

    local camera = scene:get_camera();

    camera:set_pos(0, 5.0, 0.0);

    entity:get_transform().pos = camera:get_pos();
    scene.globals.player.pos = entity:get_transform().pos;
end

function update()
    local dir = {
        x = 0,
        y = 0,
        z = 0
    };
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
        velocity = (10 * Time.delta_time) * 4;
    else
        velocity = 10 * Time.delta_time;
    end

    if (Input:get_key(KEYS["KEY_LEFT_SHIFT"])) then
        velocity = velocity * 10;
    end

    local camera = scene:get_camera();
    local old_pos = camera:get_pos();
    local forward = camera:get_forward();
    local right = camera:get_right();

    local new_pos = Vector3.new(old_pos.x + (forward.x * dir.z + right.x * dir.x) * velocity,
        old_pos.y + (forward.y * dir.z + right.y * dir.x) * velocity,
        old_pos.z + (forward.z * dir.z + right.z * dir.x) * velocity)

    entity:get_transform().pos = Vector3.new(new_pos.x, new_pos.y, new_pos.z);

    camera:set_pos(new_pos.x, new_pos.y, new_pos.z);

    entity:get_transform().rot.y = math.deg(math.atan(forward.x, forward.z));

    local delta_mouse_pos = Input:get_delta_mouse_pos();
    camera:set_yaw(camera:get_yaw() + delta_mouse_pos.x * 0.05);

    local pitch = camera:get_pitch() + delta_mouse_pos.y * 0.05;
    if (pitch > 89.0) then
        pitch = 89.0;
    elseif (pitch < -89.0) then
        pitch = -89.0;
    end

    camera:set_pitch(pitch);
    entity:get_transform().rot.x = -pitch;

    -- Check if the E key is pressed
    if (Input:get_key_down(KEYS["KEY_E"])) then
        local obj, _ = test_raycast(camera:get_forward(), 20.0);

        if (obj ~= nil) then
            print(obj:get_tag())
            local interactable = obj:get_script("interactable");
            if (interactable ~= nil) then
                interactable.interact(entity);
            end
        end
    end
end

function test_raycast(direction, max_distance)
    local origin = entity:get_transform().pos;
    local hit_info = RayCastHitInfo.new();

    local obj = scene:raycast(origin, direction, max_distance, hit_info);
    return obj, hit_info;
end
