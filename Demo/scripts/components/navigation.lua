MAX_DISTANCE          = 0.0;

MOVE_SPEED            = 0.0;
ROTATE_SPEED          = 0.0;
BRAKE_FORCE           = 0.0;

STATE                 = "idle"; -- states: "flee", "chase", "wander", "idle"

local target          = Vector3.new();
local can_move        = false;

local agent           = nil;

local position        = Vector3.new();
local rotation        = Vector3.new();
local forward         = Vector3.new();

local wander_timer    = 0.0;
local wander_duration = 10.0;

local chase_timer     = 0.0;
local chase_update_interval = 0.2;

local hitting_wall    = false;

function set_target(new_target, lookAway)
    target = new_target;
    look_at(target, lookAway)
end

function get_target()
    return target;
end

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

function look_at(target, away)
    local lookDir = Vector3.new();
    if (not away) then
        lookDir = normalize(target - position);
    else
        lookDir = normalize(position - target);
    end
    lookDir.y = 0.0;

    local angle = math.deg(atan2(lookDir.x, lookDir.z));

    rotation.y = angle;
end

function flee()
    if (distance(target, position) < MAX_DISTANCE) then
        STATE = "idle"
    end
    can_move = true;
end

function chase()
    if (distance(target, position) < MAX_DISTANCE) then
        STATE = "idle"
    end
    can_move = true;

    chase_timer = chase_timer + Time.tick_rate;

    if (chase_timer >= chase_update_interval) then
        set_target(target, false);
        chase_timer = 0.0; -- reset the timer
    end
end

function wander()
    can_move = true;

    wander_timer = wander_timer + Time.tick_rate;

    local dst = distance(target, position);

    if (wander_timer >= wander_duration or dst < MAX_DISTANCE) then
        local rand_target = gen_random_target();
        set_target(rand_target, false);
        wander_timer = 0.0;
        set_state("idle");
        --print("wander timer expired: new random target");
    end
end

function idle()
    can_move = false;
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
end

function tick()
    trans    = entity:get_transform();

    position = trans.pos;
    rotation = trans.rot;
    forward  = trans.forward;

    if(hitting_wall) then
        local hitting_dir = entity:get_transform().forward;
        local rotated_dir = Vector3.new(-hitting_dir.x, -hitting_dir.y, -hitting_dir.z)

        rotation.y = wrap_angle(rotation.y + 180.0);

        local impulse = rotated_dir * 1.5;
        agent:apply_impulse(impulse);
        local rand_target = gen_random_target();
        
        set_target(rand_target, false);
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

