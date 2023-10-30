local attacked_entities = {};
local knocked_timers = {}; --same index as used above

local knock_cooldown <const> = 3 * 60;

FORCE = Vector3.new(30, 30, 30);


function init()

end

local function re_orient(entity)
    local trans = entity:get_transform();
    trans.pos.y = trans.pos.y + 5;
    trans.rot = Vector3.new(0, 0, 0);


    local rb = entity:get_rigid();
    rb.linearVelocity = Vector3.new(0, 0, 0);
    rb.angularVelocity = Vector3.new(0, 0, 0);
    rb.rot_lock = BVector3.new(true, true, true);
end

local function removeAt(t, index)
    for i = index, #t - 1 do
        t[i] = t[i + 1]
    end
    t[#t] = nil
end

function tick()
    local i = 1
    while i <= #attacked_entities do
        knocked_timers[i] = knocked_timers[i] - 1;

        if (knocked_timers[i] < 0) then
            re_orient(attacked_entities[i]);
            removeAt(attacked_entities, i);
            removeAt(knocked_timers, i);
        else --doing this because I removed element and it would skip an index
            i = i + 1
        end
    end
end

function attack(entity)
    attacked_entities[#attacked_entities + 1] = entity;
    knocked_timers[#knocked_timers + 1] = knock_cooldown;



    local rb = entity:get_rigid();
    rb.rot_lock = BVector3.new(false, false, false);

    local forward = entity:get_transform().forward;

    rb:apply_impulse(FORCE);
    print(forward.x .. " " .. forward.y .. " " .. forward.z);
    rb:add_torque(Vector3.new(math.random(-5, 5), math.random(-5, 5), math.random(-5, 5)));
end
