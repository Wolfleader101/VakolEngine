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
local forward  = Vector3.new();

local target_direction = Vector3.new();

local wander_target = Vector3.new();
local wander_timer = 0.0;
local wander_duration = 10.0;

local idle_timer = 0.0;
local idle_duration = 5.0;

function set_state(new_state)
    state = new_state;
end

function get_state()
    return state;
end

function gen_random_target()
    local minX, maxX, minZ, maxZ = -100, 100, -100, 100;

    local x = math.random(minX, maxX);
    local y = 0.0;
    local z = math.random(minZ, maxZ);

    return Vector3.new(x, y, z);
end

function flee()
    can_move = true;
end

function chase()
    can_move = true;
end

function wander()
    can_move = true;

    wander_timer = wander_timer + Time.delta_time;

    if (wander_timer >= wander_duration) then
        wander_timer = 0.0;

        wander_target = gen_random_target();
        set_state("idle");
    end
end

function idle()
    can_move = false;

    agent.linearVelocity = Vector3.new();

    idle_timer = idle_timer + Time.delta_time;

    if (idle_timer >= idle_duration) then
        idle_timer = 0.0;

        wander_target = gen_random_target();
        set_state("wander");
    end
end

function accelerate()
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

    trans = entity:get_transform();

    position = trans.pos;
    rotation = trans.rot;
    forward  = trans.forward;

    wander_target = gen_random_target();
end

function update()
    trans = entity:get_transform();

    position = trans.pos;
    rotation = trans.rot;
    forward  = trans.forward;

    if (Input:get_key_down(KEYS["KEY_1"])) then
        rotation.y = 90.0;
    end
end

function tick()
    if (state == "wander") then
        wander();
    elseif (state == "flee") then
        flee();
    elseif (state == "chase") then
        chase();
    elseif (state == "idle") then
        idle();
    end
end

function phys_update()
    if (can_move) then
        accelerate();
    end
end
