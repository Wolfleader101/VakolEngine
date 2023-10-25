TARGET = Vector3.new();
MAX_DISTANCE = 0.0;

MOVE_SPEED = 0.0;
BRAKE_FORCE = 0.0;

local state = "wander"; -- states: "flee", "chase", "wander", "wait"

local can_move = false;

local agent = nil;

local position = Vector3.new();
local rotation = Vector3.new();

local target_direction = Vector3.new();

local wander_target = Vector3.new();
local wander_timer = 0.0;
local wander_duration = 10.0;

function set_state(new_state)
    state = new_state;
end

function gen_random_target()
    local minX, maxX, minZ, maxZ = -80, 80, -80, 80;

    local x = math.random(minX, maxX);
    local y = position.y;
    local z = math.random(minZ, maxZ);

    return Vector3.new(x, y, z);
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

function flee(target)
    can_move = true;

    local target_direction = normalize(position - target);
    target_direction.y = 0.0;

    local targetAngle = math.deg(atan2(target_direction.x, target_direction.z));

    rotation.y = targetAngle;
end

function chase(target)
    can_move = true;

    local target_direction = normalize(target - position);
    target_direction.y = 0.0;

    local targetAngle = math.deg(atan2(target_direction.x, target_direction.z));

    rotation.y = targetAngle;
end

function wander()
    can_move = true;

    wander_timer = wander_timer + Time.delta_time;

    if (wander_timer >= wander_duration) then
        wander_target = gen_random_target();
        wander_timer = 0.0;
    end

    target_direction = normalize(wander_target - position);
    target_direction.y = 0.0;
    
    local targetAngle = math.deg(atan2(target_direction.x, target_direction.z));

    rotation.y = targetAngle;
end

function idle()
    can_move = false;
end

function move(force)
    agent:apply_impulse(force);
end

function accelerate()
    local movement = target_direction * MOVE_SPEED;

    move(movement);
end

function brake()
    local brakeDir = agent.linearVelocity:normalize():negate();
    local brakeForce = brakeDir * BRAKE_FORCE;

    move(brakeForce);
end

function update()
    trans = entity:get_transform();
    
    position = trans.pos;
    rotation = trans.rot;

    forward = trans.forward;

    if (state == "wander") then
        wander();
    elseif (state == "flee") then
        flee(TARGET);
    elseif (state == "chase") then
        chase(TARGET);
    elseif (state == "idle") then
        idle();
    end
end

function phys_update()
    if (can_move) then
        --local speed = agent.linearVelocity:length();

        local distance = target_direction:length_sq();

        print("distance to target: " .. distance);

        if (distance > MAX_DISTANCE) then
            accelerate();
        else
            brake();
        end
    end
end
