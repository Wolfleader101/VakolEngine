local dir = Vector3.new(0, 0, 0);
local use_noclip = false;
local held_item = nil;

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
        if (held_item ~= nil) then
            if (held_item:get_script("interactable") ~= nil) then
                held_item:get_script("interactable").interact(entity);
            end
            held_item = nil;
            return;

        end
        
        check_for_object();
    end

    if (use_noclip) then
        no_clip_move();
        return;
    end

    local camera = scene:get_camera();
    local forward = camera:get_forward();

    local new_pos = entity:get_transform().pos

    camera:set_pos(new_pos.x, new_pos.y + 2, new_pos.z);

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
    camera_forward.y = 0;

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


function check_for_object()
    -- start origin from camera pos + add some distance so that it doenst hit player
    local origin = camera:get_pos() + camera:get_forward() * 2;
    local dir = camera:get_forward(); 
    local distance = 10.0;
    local hit_info = RayCastHitInfo.new();

    local obj = scene:raycast(origin, dir, distance, hit_info);

    if (obj ~= nil) then
        print(obj:get_tag())
        local affordance = obj:get_script("affordance");
        local interactable = obj:get_script("interactable");

        if (affordance ~= nil and affordance.AFFORDANCES.HOLDING == 1.0 and interactable ~= nil) then
        print(obj:get_tag() .. " Affords Holding")
        held_item = obj;
        interactable.interact(entity);
    end
    end
end