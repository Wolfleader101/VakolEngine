DESTINATION = Vector3.new();

local state = "wander"; -- states: "flee", "chase", "wander", "wait"

local targetDir = Vector3.new();

local can_move = false;
local can_rotate = false;

local agent = nil;
local target = Vector3.new();

local position = Vector3.new();
local rotation = Vector3.new();

local move_speed = 0.05;
local rotate_speed = 0.1;

function set_state(new_state)
    state = new_state;
end

function init()
    agent = entity:get_rigid();
end

function flee()
    can_move = true;

    targetDir = normalize(position - target);
    targetDir.y = 0.0;

    local targetAngle = math.deg(atan2(targetDir.x, targetDir.z));

    rotation.y = targetAngle;
end

function chase()
    can_move = true;

    targetDir = normalize(target - position);
    targetDir.y = 0.0;

    local targetAngle = math.deg(atan2(targetDir.x, targetDir.z));

    rotation.y = targetAngle;
end

function idle()
    can_move = false;
end

function update()
    trans = entity:get_transform();

    target = scene:get_camera():get_pos();
    
    position = trans.pos;
    rotation = trans.rot;
    forward = trans.forward;

    if (Input:get_key_down(KEYS["KEY_1"])) then
        set_state("flee");
    end

    if (Input:get_key_down(KEYS["KEY_2"])) then
        set_state("chase");
    end

    if (Input:get_key_down(KEYS["KEY_3"])) then
        set_state("wander");
    end

    if (Input:get_key_down(KEYS["KEY_4"])) then
        set_state("idle");
    end

    if (state == "flee") then
        flee();
    elseif (state == "chase") then
        chase();
    elseif (state == "idle") then
        idle();
    end
end

function phys_update()
    if (can_move) then
        local movement = targetDir * move_speed;
        agent:apply_impulse(movement);
    end
end
