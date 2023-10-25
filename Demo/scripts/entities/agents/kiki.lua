local emotions = nil;
local nav = nil;

local target = Vector3.new();

function init()
    entity:add_model("assets/models/ai/kiki/kiki.obj", 1);

    entity:add_script("emotions", "components/emotion.lua");
    emotions = entity:get_script("emotions");

    rb = entity:add_rigid();
    rb.type = BodyType.Dynamic;
    rb.rot_lock = BVector3.new(true, true, true);
    rb.hasGravity = false;

    entity:add_box_collider(Vector3.new(0.9, 1.75, 0.3));

    entity:add_script("navigation", "components/navigation.lua");
    nav = entity:get_script("navigation");

    nav.set_state("idle");

    nav.MOVE_SPEED = 0.025;
    nav.ROTATE_SPEED = 2.5;
    nav.BRAKE_FORCE = 1.0;
end

function tick()
    
end

function update()
    
end
