TARGET                = Vector3.new();
MAX_DISTANCE          = 0.0;

MOVE_SPEED            = 0.0;
ROTATE_SPEED          = 0.0;
BRAKE_FORCE           = 0.0;

STATE                 = "idle"; -- states: "flee", "chase", "wander", "idle"

local can_move        = false;

local agent           = nil;

local position        = Vector3.new();
local rotation        = Vector3.new();
local forward         = Vector3.new();

local wander_target   = Vector3.new();
local wander_timer    = 0.0;
local wander_duration = 10.0;

local hitting_wall    = false;


function wrap_angle(angle)
    if angle > 180.0 then
        angle = angle - 360.0
    elseif angle < -180.0 then
        angle = angle + 360.0
    end

    return angle;
end

function set_state(new_state)
    STATE = new_state;
end

function get_state()
    return STATE;
end

function gen_random_target()
    local minX, maxX, minZ, maxZ = -40, 40, -40, 40;

    local x = math.random(minX, maxX);
    local y = 0.0;
    local z = math.random(minZ, maxZ);

    return Vector3.new(x, y, z);
end

function look_at(target)
    local lookDir = normalize(target - position);
    lookDir.y = 0.0;

    local desired_angle = math.deg(atan2(lookDir.x, lookDir.z));
    local current_angle = rotation.y;

    -- angle wrapping
    desired_angle = (desired_angle + 180) % 360 - 180;
    current_angle = (current_angle + 180) % 360 - 180;

    local angleDiff = desired_angle - current_angle;
    
    -- angle difference is within -180 to 180 degrees
    if angleDiff > 180 then
        angleDiff = angleDiff - 360;
    elseif angleDiff < -180 then
        angleDiff = angleDiff + 360;
    end

    local proportionalFactor = 0.01;
    local dampingFactor = 0.4;     -- closer to 1 for less damping
    local maxTorque = 4;
    local deadZone = 35;             -- (degrees)

    -- if the angle difference is within the dead zone
    if math.abs(angleDiff) > deadZone then
        -- proportional control
        local torqueValue = angleDiff * proportionalFactor;

        -- clamping the torque
        torqueValue = math.min(math.abs(torqueValue), maxTorque);

        -- damping
        torqueValue = torqueValue * dampingFactor;

        -- Create torque vector
        local torque = Vector3.new(0, torqueValue, 0);

        -- Apply torque
        agent:add_torque(torque);
    else
        local inv_current_angular_velocity = Vector3.new(-agent.angularVelocity.x, -agent.angularVelocity.y, -agent.angularVelocity.z);
        local damping_torque = inv_current_angular_velocity * 0.003
        agent:add_torque(damping_torque)
    end
end



function flee()
    if (distance(TARGET, position) < MAX_DISTANCE) then
        STATE = "idle"
    end
    can_move = true;

    smooth_look_at(TARGET, true);
end

function chase()
    if (distance(TARGET, position) < MAX_DISTANCE) then
        STATE = "idle"
    end
    can_move = true;

    look_at(TARGET);
end

function wander()
    can_move = true;

    wander_timer = wander_timer + Time.tick_rate;

    local dst = distance(wander_target, position);

    look_at(wander_target);

    if (wander_timer >= wander_duration or dst < MAX_DISTANCE) then
        wander_target = gen_random_target();
        wander_timer = 0.0;
        set_state("idle");
        print("wander timer expired: new random target");
    end
end

function idle()
    can_move = false;
    -- agent.linearVelocity = Vector3.new();
end

local function accelerate()
    local movement = forward * MOVE_SPEED;

    move(movement);
end

function move(force)
    agent:apply_impulse(force);
end

function init()
    agent = entity:get_rigid();

    if (agent == nil) then
        print_warn("No rigidbody component detected on agent!");
    end

    trans         = entity:get_transform();

    position      = trans.pos;
    rotation      = trans.rot;
    forward       = trans.forward;

    wander_target = gen_random_target();
end

function tick()
    trans    = entity:get_transform();

    position = trans.pos;
    rotation = trans.rot;
    forward  = trans.forward;

    --if (Input:get_key_down(KEYS["KEY_1"])) then
    --    look_at(Vector3.new(0.0, 1.0, -40.0));
    --elseif (Input:get_key_down(KEYS["KEY_2"])) then
    --    look_at(Vector3.new(0.0, 1.0, 40.0));
    --elseif (Input:get_key_down(KEYS["KEY_3"])) then
    --    look_at(Vector3.new(-40.0, 1.0, 0.0));
    --elseif (Input:get_key_down(KEYS["KEY_4"])) then
    --    look_at(Vector3.new(40.0, 1.0, 0.0));
    --end
    if(hitting_wall) then
        -- local inverse_forward = Vector3.new(-hitting_dir.x, -hitting_dir.y, -hitting_dir.z);
        local hitting_dir = entity:get_transform().forward;
        local rotated_dir =  Vector3.new(-hitting_dir.x, -hitting_dir.y, -hitting_dir.z)-- rotate_vec3(entity:get_transform().forward, 180.0, Vector3.new(0.0, 1.0, 0.0));
        
        -- look_at(rotated_dir);
        local impulse = rotated_dir * 1.5;
        agent:apply_angular_impulse(Vector3.new(1.0, 0.0, 0.0), Vector3.new(0.0, 1.0, 0.0))
        agent:apply_impulse(impulse);
        

        
        wander_target = gen_random_target();
        wander_timer = 0.0;
        hitting_wall = false;
    elseif (STATE == "wander") then
        wander();
    elseif (STATE == "flee") then
        flee();
    elseif (STATE == "chase") then
        chase();
    elseif (STATE == "idle") then
        idle();
    end
end

local function check_for_wall()
    local origin = entity:get_transform().pos;  -- Get entity's current position
    local dir = entity:get_transform().forward; -- Get entity's current forward direction
    local distance = 2.0;                      -- Length of the ray, you can adjust this value
    local hit_info = RayCastHitInfo.new();

    local other_ent = scene:raycast(origin, dir, distance, hit_info);

    if (other_ent == nil) then
        return;
    end
    
    local affordanceComp = other_ent:get_script("affordance");
    if(affordanceComp ~= nil and affordanceComp.AFFORDANCES.WALKING == 1.0) then
        return;
    end

    if (hit_info.rigidbody.type == BodyType.Static) then
        hitting_wall = true;
        -- print("Hit a wall!");
        -- wander_target = gen_random_target();
        -- wander_timer = 0.0;
        -- local inverse_normal = Vector3.new(-hit_info.normal.x, -hit_info.normal.y, -hit_info.normal.z);
        -- smooth_look_at(inverse_normal, false);

        -- move away from the wall
    else
        hitting_wall = false;
    end
    
end

function phys_update()
    if (can_move) then
        check_for_wall();
        accelerate();
    end
end

