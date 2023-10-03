function init()
    local floor = scene:create_entity("Floor", "");
    floor:get_transform().scale = Vector3.new(100.0, 1.0, 100.0);
    floor:add_model("coreAssets/models/cube.obj", 1);

    local rb1 = floor:add_rigid();
    rb1.mass = 1;
    rb1.type = BodyType.Static;
    floor:add_box_collider(Vector3.new(100.0, 1.0, 100.0));
    
    local cube = scene:create_entity("Cube", "");
    cube:get_transform().pos = Vector3.new(10.0, 20.0, 0.0);
    cube:get_transform().rot = Vector3.new(0.0, 0.0, 0.0);
    cube:add_model("coreAssets/models/cube.obj", 1);

    local rb = cube:add_rigid();
    rb.mass = 1;
    rb.bounciness = 0.7;
    rb.type = BodyType.Dynamic;
    cube:add_box_collider(Vector3.new(1.0, 1.0, 1.0));
end