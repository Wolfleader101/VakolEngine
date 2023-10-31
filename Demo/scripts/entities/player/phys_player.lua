local dir = Vector3.new(0, 0, 0);
local use_noclip = false;

function init()
    print("Initialising Physics Player");

    entity:get_transform().pos = Vector3.new(5, 5, -15);

    local rb = entity:add_rigid();
    rb.rot_lock = BVector3.new(true, true, true);
    rb.bounciness = 0.3;
    rb.mass = 1;
    entity:add_box_collider(Vector3.new(1.0, 1.0, 1.0));
end

local camera = scene:get_camera();

function update()
    if (Input:get_key_down(KEYS["KEY_Z"])) then
        use_noclip = not use_noclip;
    end

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

    if (use_noclip) then
        no_clip_move();
        return;
    end

    local camera = scene:get_camera();
    local forward = camera:get_forward();

    local new_pos = entity:get_transform().pos

    camera:set_pos(new_pos.x, new_pos.y + 4, new_pos.z);

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
end

local MAX_VEL <const> = 20;
local keyHit = false;

function phys_update()
    if (use_noclip) then
        return
    end


    local rb = entity:get_rigid();
    local camera = scene:get_camera();
    local camera_forward = camera:get_forward():normalize();

    local camera_right = camera:get_right():normalize();


    if (Input:get_key(KEYS["KEY_W"])) then
        dir = dir + camera_forward;
        keyHit = true;
    end
    if (Input:get_key(KEYS["KEY_S"])) then
        dir = dir - camera_forward;
        keyHit = true;
    end
    if (Input:get_key(KEYS["KEY_A"])) then
        dir = dir - camera_right;
        keyHit = true;
    end
    if (Input:get_key(KEYS["KEY_D"])) then
        dir = dir + camera_right;
        keyHit = true;
    end

    if (keyHit) then
        -- Multiply by force magnitude here, for example, 100
        dir = dir * 1.5;
        if (rb.linearVelocity:magnitude() < MAX_VEL) then
            --print(dir.x .. " " .. dir.y .. " " .. dir.z);
            rb:apply_impulse(dir);
        end

        dir = Vector3.new(0, 0, 0);
        keyHit = false;
        return
    end

    rb.linearVelocity.x = 0;
    rb.linearVelocity.z = 0;
end

function no_clip_move()
    local noclip_dir = { x = 0, y = 0, z = 0 };
    local velocity = 0;

    if (Input:get_key(KEYS["KEY_W"])) then
        noclip_dir.z = 1;
    end
    if (Input:get_key(KEYS["KEY_S"])) then
        noclip_dir.z = -1;
    end
    if (Input:get_key(KEYS["KEY_A"])) then
        noclip_dir.x = -1;
    end
    if (Input:get_key(KEYS["KEY_D"])) then
        noclip_dir.x = 1;
    end

    if (Input:get_key(KEYS["KEY_LEFT_SHIFT"])) then
        velocity = (10 * Time.delta_time) * 4;
    else
        velocity = 10 * Time.delta_time;
    end

    if (Input:get_key(KEYS["KEY_LEFT_SHIFT"])) then
        velocity = velocity * 5;
    end

    local camera = scene:get_camera();
    local old_pos = camera:get_pos();
    local forward = camera:get_forward();
    local right = camera:get_right();

    local new_pos = Vector3.new(
        old_pos.x + (forward.x * noclip_dir.z + right.x * noclip_dir.x) * velocity,
        old_pos.y + (forward.y * noclip_dir.z + right.y * noclip_dir.x) * velocity,
        old_pos.z + (forward.z * noclip_dir.z + right.z * noclip_dir.x) * velocity
    )

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
end

function test_raycast(direction, max_distance)
    local origin = camera:get_pos();

    print(origin.x .. " " .. origin.y .. " " .. origin.z);
    local hit_info = RayCastHitInfo.new();

    local obj = scene:raycast(origin, direction, max_distance, hit_info);
    return obj, hit_info;
end
