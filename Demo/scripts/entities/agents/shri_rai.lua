
local emotions;
-- local nav;
-- local ...

local piss_particles = {}
local piss_count = 0;
local <const> MAX_PISS = 100;


local function piss()
    
    piss_count = piss_count + 1;
    piss_particles[piss_count] = scene:create_entity("piss" .. piss_count, "");
end

function init()

    entity:add_model("assets/models/ai/shri/shri.fbx", 1);
    entity:get_transform().scale = Vector3.new(0.015, 0.015, 0.015);

    entity:add_script("emotions", "components/emotion.lua");
    emotions = entity:get_script("emotions");

    entity:add_rigid();
    entity:add_box_collider(Vector3.new(0.9 , 1.75, 0.3));


    
end

function tick()
    
end