TARGET = Vector3.new();
MAX_DISTANCE = 0.0;

MOVE_SPEED = 0.0;
ROTATE_SPEED = 0.0;
BRAKE_FORCE = 0.0;

STATE = "chase"; -- states: "flee", "chase", "wander", "wait"

local can_move = false;

local agent = nil;

local position = Vector3.new();
local rotation = Vector3.new();

local target_direction = Vector3.new();

local wander_target = Vector3.new();
local wander_timer = 0.0;
local wander_duration = 10.0;

function wrap_angle(angle)
    if (angle > 180) then
        angle = angle - 360;
    elseif (angle < -180) then
        angle = angle + 360;
    end

    return angle
end

function set_state(new_state)
    STATE = new_state;
end

function get_state()
    return STATE;
end

function gen_random_target()
    local minX, maxX, minZ, maxZ = -20, 20, -20, 20 ;

    local x = math.random(minX, maxX);
    local y = position.y;
    local z = math.random(minZ, maxZ);

    return Vector3.new(x, y, z);
end

function look_at(target, away)
    if (not away) then
        target_direction = normalize(target - position);
    else
        target_direction = normalize(position - target)
    end

    target_direction.y = 0.0;

    local prevRotY = rotation.y;
    
    local targetAngle = math.deg(atan2(target_direction.x, target_direction.z));

    local angleDiff = (targetAngle - prevRotY);

    local factor = math.min(1, ROTATE_SPEED * Time.delta_time);

    rotation.y = lerp(prevRotY, prevRotY + angleDiff, factor);
end

function init()
    agent = entity:get_rigid();

    if (agent == nil) then
        print_warn("No rigidbody component detected on agent!");
    end

    trans = entity:get_transform();

    position = trans.pos;
    rotation = trans.rot;

    wander_target = gen_random_target();
end

function flee()
    can_move = true;

    look_at(TARGET, true);
end

function chase()
    can_move = true;

    look_at(TARGET, false);
end

function wander()
    can_move = true;

    wander_timer = wander_timer + Time.delta_time;

    if (wander_timer >= wander_duration) then
        wander_target = gen_random_target();
        wander_timer = 0.0;
    end

    look_at(wander_target);
end

function idle()
    can_move = false;

    --agent.linearVelocity = Vector3.new();
end

function move(force)
    agent:apply_impulse(force);
end

function accelerate()
    local movement = target_direction * MOVE_SPEED;

    move(movement);
end

function tick()
    trans = entity:get_transform();
    
    position = trans.pos;
    rotation = trans.rot;

    if (state == "wander") then
        wander();
    elseif (STATE == "flee") then
        flee();
    elseif (STATE == "chase") then
        chase();
    elseif (STATE == "idle") then
        idle();
    end
end

function phys_update()
    if (can_move) then
        local distance = target_direction:length();

        if (distance > MAX_DISTANCE) then
            accelerate();
        else
            set_state("idle");
        end
    end
end
