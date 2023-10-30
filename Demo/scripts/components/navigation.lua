TARGET                = Vector3.new();
MAX_DISTANCE          = 0.0;

MOVE_SPEED            = 0.0;
ROTATE_SPEED          = 0.0;
BRAKE_FORCE           = 0.0;

STATE                 = "chase"; -- states: "flee", "chase", "wander", "wait"

local can_move        = false;

local agent           = nil;

local position        = Vector3.new();
local rotation        = Vector3.new();
local forward         = Vector3.new();

local wander_target   = Vector3.new();
local wander_timer    = 0.0;
local wander_duration = 10.0;

local hitting_wall    = false;
local hitting_dir     = Vector3.new();

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

function smooth_look_at(target, away)
    local lookDir = Vector3.new();

    if (not away) then
        lookDir = normalize(target - position);
    else
        lookDir = normalize(position - target);
    end
    lookDir.y = 0.0;

    local angle = math.deg(atan2(lookDir.x, lookDir.z));

    local angleDiff = angle - rotation.y;

    if (angleDiff > 180.0) then
        angleDiff = angleDiff - 360.0;
    elseif (angleDiff < -180.0) then
        angleDiff = angleDiff + 360.0;
    end

    local factor = ROTATE_SPEED * Time.delta_time;

    rotation.y = rotation.y + factor * angleDiff;
end

function look_at(target)
    local lookDir = normalize(target - position);
    lookDir.y = 0.0;

    local angle = math.deg(atan2(lookDir.x, lookDir.z));

    rotation.y = angle;
end

function flee()
    can_move = true;

    smooth_look_at(TARGET, true);
end

function chase()
    if (distance(TARGET, position) < MAX_DISTANCE) then
        STATE = "idle"
    end
    can_move = true;

    smooth_look_at(TARGET, false);
end

function wander()
    can_move = true;

    wander_timer = wander_timer + Time.delta_time;

    local dst = distance(wander_target, position);

    smooth_look_at(wander_target, false);

    if (wander_timer >= wander_duration or dst < MAX_DISTANCE) then
        wander_target = gen_random_target();
        wander_timer = 0.0;
        set_state("idle");
        print("wander timer expired: new random target");
    end
end

function idle()
    can_move = false;
    agent.linearVelocity = Vector3.new();
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
        local inverse_forward = Vector3.new(-hitting_dir.x, -hitting_dir.y, -hitting_dir.z);
        look_at(inverse_forward);
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
    local distance = 0.5;                      -- Length of the ray, you can adjust this value
    local hit_info = RayCastHitInfo.new();

    local other_ent = scene:raycast(origin, dir, distance, hit_info);

    if (other_ent ~= nil and hit_info.rigidbody.type == BodyType.Static) then
        hitting_wall = true;
        hitting_dir = dir; -- save the direction we hit the wall
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

