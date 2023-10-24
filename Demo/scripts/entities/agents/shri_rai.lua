
local emotions;
-- local nav;
-- local ...

local piss_particles = {}
local piss_count = 0;

local function piss()

end

function init()
    print("shri is in the building");
    entity:add_model("assets/models/ai/shri/shri.fbx", 1);
    entity:get_transform().scale = Vector3.new(0.015, 0.015, 0.015);

    entity:add_script("emotions", "components/emotion.lua");
    emotions = entity:get_script("emotions");

    entity:add_rigid();
    entity:add_box_collider(Vector3.new(0.9 , 1.75, 0.3));



    --emotions = entity:get_script("emotions");
    
end

function tick()
    
end