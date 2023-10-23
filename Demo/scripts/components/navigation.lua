DESTINATION = Vector3.new();

local state = "wander"; -- states: "navigate", "wander", "wait"

local forward = Vector3.new(0.0, 0.0, -1.0);

local can_move = false;

local agent = nil;

local move_speed = 0.1;

function set_state(new_state)
    state = new_state;
end

function init()
    agent = entity:get_rigid();
end

function gen_rand_position()
    local minX, maxX, minZ, maxZ = -25, 25, -25, 25;

    local x = math.random(minX, maxX);
    local y = entity:get_transform().pos.y;
    local z = math.random(minZ, maxZ);

    return Vector3.new(x, y, z);    
end

function update()
    trans = entity:get_transform();
    
    local pos = trans.pos;
    local rot = trans.rot;
    forward = trans.forward;

    if (Input:get_key_down(KEYS["KEY_SPACE"])) then
        local target = scene:get_camera():get_pos();

        local direction = normalize(target - pos);
        direction.y = 0.0;

        local angle = math.deg(atan2(direction.x, direction.z));

        rot.y = angle;
    end
    
    if (Input:get_key(KEYS["KEY_E"])) then
        can_move = true;
    else
        can_move = false;
    end
end

function phys_update()
    if (can_move) then
        local movement = forward * move_speed;
        agent:apply_impulse(movement);
    end
end
