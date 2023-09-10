function init()
    print("Initialising Physics Player");

    local rb = entity:add_rigid();
    rb.bounciness = Vector3.new(0.8, 0.3, 0.9);
	rb.mass = 1;
	entity:add_box_collider(Vector3.new(1.0, 1.0, 1.0));
end

function update()
    local camera = scene:get_camera();
    local forward = camera:get_forward();

    local new_pos = entity:get_transform().pos

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

function phys_update()
    local rb = entity:get_rigid();
    local dir = Vector3.new(0, 0, 0);  -- Assuming Vector3 has a constructor
    local camera = scene:get_camera();
    local camera_forward = camera:get_forward():normalize();
    local camera_right =  camera:get_right():normalize();
    
    if (Input:get_key(KEYS["KEY_W"])) then
        dir = dir + camera_forward;
    end
    if (Input:get_key(KEYS["KEY_S"])) then
        dir = dir - camera_forward;
    end
    if (Input:get_key(KEYS["KEY_A"])) then
        dir = dir - camera_right;
    end
    if (Input:get_key(KEYS["KEY_D"])) then
        dir = dir + camera_right;
    end
    
    local jump_force = 5;
    if (Input:get_key(KEYS["KEY_SPACE"]) and is_grounded()) then
        dir.y = dir.y + jump_force;
    end

    -- Multiply by force magnitude here, for example, 100
    dir = dir * 50;

    rb:add_force(dir);
end

function is_grounded()
    local origin = entity:get_transform().pos;  -- Get entity's current position
    local downward = Vector3.new(0, -1, 0);  -- Downward direction
    local distance = 0.5;                      -- Length of the ray, you can adjust this value
    local hit_info = RayCastHitInfo.new();

    -- local didHit = scene:raycast(origin, downward, distance, hit_info);

    -- print(hit_info.distance);
    
    return scene:raycast(origin, downward, distance, hit_info);
end