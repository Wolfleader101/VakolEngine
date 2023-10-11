function init()
    add_floor();
    add_cube();
    --add_cube2();
    add_sphere();
end

function add_sphere()
    local sphere = scene:create_entity("Sphere", "");
    sphere:add_model("coreAssets/models/sphere.obj", 1.0);

    sphere:get_transform().pos = Vector3.new(0.0, 10.0, 0.0);

    sphere:add_rigid();
    sphere:add_sphere_collider(1.0);
end

function add_cube()
    local cube = scene:create_entity("Cube 1", "");

    cube:get_transform().pos = Vector3.new(0.0, 7.5, 0.0);
    cube:get_transform().rot = Vector3.new(0.0, 0.0, 0.0);

    cube:add_model("coreAssets/models/cube.obj", 1.0);

    local rb = cube:add_rigid();
    cube:add_box_collider(Vector3.new(1.0, 1.0, 1.0));
end

function add_cube2()
    local cube2 = scene:create_entity("Cube 2", "");

    cube2:get_transform().pos = Vector3.new(0.0, 15.0, 0.0);
    cube2:get_transform().rot = Vector3.new(0.0, 0.0, 0.0);

    cube2:add_model("coreAssets/models/cube.obj", 1.0);

    local rb = cube2:add_rigid();
    cube2:add_box_collider(Vector3.new(1.0, 1.0, 1.0));
end

function add_floor()
    local floor = scene:create_entity("Floor", "");

    floor:get_transform().scale = Vector3.new(5.0, 1.0, 5.0)

    floor:add_model("assets/models/digital_twin/GROUND.fbx", 0.02);
    local rb = floor:add_rigid();
    
    rb.type = BodyType.Static;
    floor:add_box_collider(Vector3.new(500.0, 0.1, 500.0));
end

function update()
end