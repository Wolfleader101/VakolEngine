function init()

    entity:add_model("assets/models/agents/bob.glb", 1.0e-02);
    entity:get_transform().pos = Vector3.new(0, 5, 0); -- shouldnt be here
    local rb = entity:add_rigid();
    rb.mass = 1;
    rb.rot_lock = BVector3.new(true, true, true);
    -- entity:add_box_collider(Vector3.new(0.75, 1.0, 0.75));
    entity:add_sphere_collider(1);
    entity:add_script("emotions", "components/emotion.lua");

    entity:add_script("navigation", "components/navigation.lua");
    nav = entity:get_script("navigation");

    target = Vector3.new(-80, 0, 80);

    nav.DESTINATION.x = target.x
    nav.DESTINATION.y = target.y
    nav.DESTINATION.z = target.z

    nav.set_state("wander");

end
