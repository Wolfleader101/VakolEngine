local emotions;
local piss;
local nav;

local target;


local states <const> = { "flee", "chase", "wander", "idle" };

local piss_locations = { Vector3.new(17, 2.2, 137), Vector3.new(1.4, 1.5, 36.5), Vector3.new(32, 2.5, 20) };
local piss_lookat = { Vector3.new(-6, 4.5, 148), Vector3.new(7.7, 2.7, 41.721), Vector3.new(-32, 2.5, 45) };
local piss_index = 3;



function init()
    entity:add_model("assets/models/ai/shri/shri.fbx", 1);

    entity:add_script("emotions", "components/emotion.lua");
    emotions = entity:get_script("emotions");

    entity:add_script("piss", "components/piss.lua");
    piss = entity:get_script("piss");

    rb = entity:add_rigid();
    rb.type = BodyType.Dynamic;
    rb.rot_lock = BVector3.new(true, true, true);
    entity:add_box_collider(Vector3.new(0.9, 1.75, 0.3));

    entity:add_script("navigation", "components/navigation.lua");
    nav = entity:get_script("navigation");

    nav.MOVE_SPEED = 0.025;
    nav.ROTATE_SPEED = 2.5;
    nav.BRAKE_FORCE = 1.0;



    nav.TARGET = piss_locations[piss_index];

    print(nav.TARGET.x .. " " .. nav.TARGET.y .. " " .. nav.TARGET.z)
    nav.MAX_DISTANCE = 1;
    nav.set_state("chase");


    --nav.set_state(states[math.random(4)]);
end

local pos;
--local pissing = false;
local pissTimer <const> = 300; -- 5 seconds
local pissTicks = 0;
function tick()
    --check emotions before (once u got that)
    --pos = entity:get_transform().pos;
    if (nav.get_state() == "idle") then
        print("here");
        pissTicks = pissTicks + 1;
        if (pissTicks < pissTimer) then
            nav.look_at(piss_locations[piss_index]);
            shoot_piss();
        else
            nav.set_state("wander");
            pissTicks = 0;
        end
    end
end

function update()
    -- if (Input:get_key(KEYS["KEY_Z"])) then
    --     pissing = true;
    -- elseif (Input:get_key(KEYS["KEY_X"])) then
    --     piss.clean();
    -- else
    --     pissing = false;
    -- end

    -- -- if (Input:get_key(KEYS["KEY_X"])) then
    -- --     piss.clean();
    -- -- end
end

function shoot_piss()
    local trans = entity:get_transform();
    local pos = Vector3.new();

    pos.x = trans.pos.x;
    pos.y = trans.pos.y - 0.8;
    pos.z = trans.pos.z;

    local forward = Vector3.new();

    forward.x = trans.forward.x;
    forward.y = trans.forward.y;
    forward.z = trans.forward.z;

    piss.piss(pos + (forward * 0.3), forward);
    -- piss.piss(pos, trans.forward * -3);
end
