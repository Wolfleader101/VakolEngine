local emotions = nil;
local nav = nil;

local target = Vector3.new();

local eye_material = nil;

function init()
    local model = entity:add_model("assets/models/ai/kiki/kiki.obj", 0.5);
    eye_material = model:get_mesh(1).material;

    entity:get_transform().pos = Vector3.new(0.0, 5.0, 0.0);

    entity:add_script("emotions", "components/emotion.lua");
    emotions = entity:get_script("emotions");

    rb = entity:add_rigid();
    rb.type = BodyType.Dynamic;
    rb.rot_lock = BVector3.new(true, true, true);

    entity:add_box_collider(model:get_half_extents(0));

    entity:add_script("navigation", "components/navigation.lua");
    nav = entity:get_script("navigation");

    target = scene:get_camera():get_pos();

    nav.TARGET = target;
    nav.MAX_DISTANCE = 5.0;

    nav.MOVE_SPEED = 0.05;
    nav.ROTATE_SPEED = 5.0;
    nav.BRAKE_FORCE = 1.0;

    nav.set_state("wander");

    eye_material:set_uv_offset(Vector2.new(0.0, 0.0));
    eye_material:set_opacity(0.0);
    eye_material:use_lighting(false);
end

function tick()
    
end


function update()
    
end