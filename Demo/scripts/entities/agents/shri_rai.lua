local emotions;
local piss;
-- local nav;
-- local ...

function init()
    entity:add_model("assets/models/ai/shri/shri.fbx", 1);

    entity:add_script("emotions", "components/emotion.lua");
    emotions = entity:get_script("emotions");

    entity:add_script("piss", "components/piss.lua");
    piss = entity:get_script("piss");

    rb = entity:add_rigid();
    rb.type = BodyType.Dynamic;
    entity:add_box_collider(Vector3.new(0.9, 1.75, 0.3));
end

local pissing = false;
function tick()
    if pissing == true then
        local trans = entity:get_transform();
        local pos = Vector3.new();

        pos.x = trans.pos.x;
        pos.y = trans.pos.y - 0.8;
        pos.z = trans.pos.z;

        piss.piss(pos, trans.forward);
        -- piss.piss(pos, trans.forward * -3);
    end
end

function update()
    if (Input:get_key(KEYS["KEY_Z"])) then
        pissing = true;
    elseif (Input:get_key(KEYS["KEY_X"])) then
        piss.clean();
    else
        pissing = false;
    end

    -- if (Input:get_key(KEYS["KEY_X"])) then
    --     piss.clean();
    -- end
end
