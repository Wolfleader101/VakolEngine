local dir = Vector3.new(0, 0, 0);
local use_noclip = false;

function init()
    print("Initialising Physics Player");

    entity:get_transform().pos.y = 2.0;

    local rb = entity:add_rigid();
    rb.bounciness = 0.3;
	rb.mass = 1;
    entity:add_box_collider(Vector3.new(1.0, 1.0, 1.0));
    rb.rot_lock = BVector3.new(true, true, true);
end

function update()

    if (Input:get_key_down(KEYS["KEY_Z"])) then
        use_noclip = not use_noclip;
    end

    if(use_noclip) then
        no_clip_move();
        return;
    end

    local camera = scene:get_camera();
    local forward = camera:get_forward();

    local new_pos = entity:get_transform().pos

    camera:set_pos(new_pos.x, new_pos.y + 1.75, new_pos.z);

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

    local is_grounded = is_grounded();

    local jump_force = 5;
    if (Input:get_key_down(KEYS["KEY_SPACE"]) and is_grounded) then
        dir.y = dir.y + jump_force;
    end
end

function phys_update()
    if (use_noclip) then
        return
    end


    local rb = entity:get_rigid();
    local camera = scene:get_camera();
    local camera_forward = camera:get_forward():normalize();
    local camera_right = camera:get_right():normalize();
    local is_grounded = is_grounded();

    if (Input:get_key(KEYS["KEY_W"]) and is_grounded) then
        dir = dir + camera_forward;
    end
    if (Input:get_key(KEYS["KEY_S"]) and is_grounded) then
        dir = dir - camera_forward;
    end
    if (Input:get_key(KEYS["KEY_A"]) and is_grounded) then
        dir = dir - camera_right;
    end
    if (Input:get_key(KEYS["KEY_D"]) and is_grounded) then
        dir = dir + camera_right;
    end

    -- Multiply by force magnitude here, for example, 100
    dir = dir * 1.2;

    rb:apply_impulse(dir);
    dir = Vector3.new(0, 0, 0);
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

function is_grounded()
    local origin = entity:get_transform().pos;  -- Get entity's current position
    local downward = Vector3.new(0, -1, 0);  -- Downward direction
    local distance = 1.0;                      -- Length of the ray, you can adjust this value
    local hit_info = RayCastHitInfo.new();
  
    local obj = scene:raycast(origin, downward, distance, hit_info);

    if (obj == nil) then
        return false;
    end
    
   return true;
end