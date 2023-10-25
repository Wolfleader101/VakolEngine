local emotions;
local piss;
-- local nav;
-- local ...

function init()

    entity:add_model("assets/models/ai/shri/shri.fbx", 1);

    trans = entity:get_transform();
    trans.pos = Vector3.new(-10, 3, -10); -- temp while testing piss
    -- trans.rot.y = 180;

    trans.scale = Vector3.new(0.015, 0.015, 0.015);

    entity:add_script("emotions", "components/emotion.lua");
    emotions = entity:get_script("emotions");

    entity:add_script("piss", "components/piss.lua");
    piss = entity:get_script("piss");

    rb = entity:add_rigid();
    rb.type = BodyType.Static;
    entity:add_box_collider(Vector3.new(0.9, 1.75, 0.3));

end

local pissing = false;
function tick()

    if pissing == true then
        trans = entity:get_transform();
        print("before" .. trans.pos.y)
        piss.piss(trans.pos, trans.forward);
    end
end

function update()
    if (Input:get_key(KEYS["KEY_Z"])) then
        pissing = true;
    else
        pissing = false;
    end

end
