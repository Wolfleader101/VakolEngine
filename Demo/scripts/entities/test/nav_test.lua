function init()
    entity:add_model("assets/models/agents/bob.glb", 1.0e-02);
    entity:get_transform().pos = Vector3.new(0.0, 5.0, 0.0); -- shouldnt be here

    local rb = entity:add_rigid();
    
    rb.mass = 10.0;
    rb.rot_lock = BVector3.new(true, true, true);
    
    entity:add_capsule_collider(0.40, 1.0);
    entity:add_script("emotions", "components/emotion.lua");
    entity:add_script("navigation", "components/navigation.lua");

    nav = entity:get_script("navigation");

    target = scene:get_camera():get_pos();

    nav.TARGET = target;
    nav.MAX_DISTANCE = 0.8;

    nav.MOVE_SPEED = 0.025;
    nav.ROTATE_SPEED = 2.5;
    nav.BRAKE_FORCE = 1.0;

    nav.set_state("chase");
end

function tick()
    target = scene:get_camera():get_pos();
    nav = entity:get_script("navigation");
    nav.TARGET = target;
end