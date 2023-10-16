function init()
    scene:create_entity("Player", "entities/player/noclip.lua")
    -- scene:create_entity("Player", "entities/player/phys_player.lua")

    -- scene:create_entity("Digital Twin", "entities/worlds/digital_twin.lua");
    -- scene:create_entity("290 World", "entities/worlds/290_world.lua");

    local path = "coreAssets/textures/Skybox/";
    local extension = ".png";

    local faces = { path .. "right" .. extension,
        path .. "left" .. extension,
        path .. "top" .. extension,
        path .. "bottom" .. extension,
        path .. "front" .. extension,
        path .. "back" .. extension };

    scene:generate_skybox(faces);


    local floor = scene:create_entity("Floor", "");
    floor:get_transform().scale = Vector3.new(100.0, 1.0, 100.0);
    floor:add_model("coreAssets/models/cube.obj", 1);

    local rb1 = floor:add_rigid();
    rb1.mass = 1;
    rb1.bounciness = 0.4;
    rb1.type = BodyType.Static;
    floor:add_box_collider(Vector3.new(100.0, 1.0, 100.0));

    -- phys_objs();
    -- falling_objs();

    for i = 1, 1000 do
        local entityName = "Cube" .. " " .. i;

        local cube = scene:create_entity(entityName, "");
        cube:get_transform().pos = Vector3.new(randomFloat(-100, 100), randomFloat(10, 50), randomFloat(-100, 100));
        cube:get_transform().rot = Vector3.new(randomFloat(-365, 365), randomFloat(-365, 365), randomFloat(-365, 365));
        local mdl = cube:add_model("coreAssets/models/cube.obj", 1);

        local mesh = mdl:get_mesh(0);

        mesh.material:set_diffuse_color(Vector4.new(randomFloat(0, 1), randomFloat(0, 1), randomFloat(0, 1), 1.0));

        local rb = cube:add_rigid();
        rb.mass = randomFloat(5,100);
        rb.bounciness = randomFloat(0, 1);
        rb.type = BodyType.Dynamic;
        cube:add_box_collider(Vector3.new(1.0, 1.0, 1.0));
    end

    --     local cube = scene:create_entity("Cube", "");
    -- cube:get_transform().pos = Vector3.new(10.0, 20.0, 0.0);
    -- cube:get_transform().rot = Vector3.new(00.0, 0.0, 45.0);
    -- cube:add_model("coreAssets/models/cube.obj", 1);

    -- local rb = cube:add_rigid();
    -- rb.mass = 100;
    -- rb.bounciness = 0.3;
    -- rb.type = BodyType.Dynamic;
    -- cube:add_box_collider(Vector3.new(1.0, 1.0, 1.0));

    -- local cube2 = scene:create_entity("Cube2", "");
    -- cube2:get_transform().pos = Vector3.new(15, 20.0, 0.0);
    -- cube2:get_transform().rot = Vector3.new(0.0, 0.0, 0.0);
    -- cube2:add_model("coreAssets/models/cube.obj", 1);

    -- local rb2 = cube2:add_rigid();
    -- rb2.mass = 10;
    -- rb2.bounciness = 0.3;
    -- rb2.type = BodyType.Dynamic;
    -- cube2:add_box_collider(Vector3.new(1.0, 1.0, 1.0));
end

function randomFloat(lower, greater)
    return lower + math.random()  * (greater - lower);
end

function falling_objs()
    local cube = scene:create_entity("Cube", "");
    cube:get_transform().pos = Vector3.new(10.0, 20.0, 0.0);
    cube:get_transform().rot = Vector3.new(00.0, 0.0, 45.0);
    cube:add_model("coreAssets/models/cube.obj", 1);

    local rb = cube:add_rigid();
    rb.mass = 10;
    rb.bounciness = 0.3;
    rb.type = BodyType.Dynamic;
    cube:add_box_collider(Vector3.new(1.0, 1.0, 1.0));

    local cube2 = scene:create_entity("Cube2", "");
    cube2:get_transform().pos = Vector3.new(10, 5.0, 0.0);
    cube2:get_transform().rot = Vector3.new(0.0, 0.0, 0.0);
    cube2:add_model("coreAssets/models/cube.obj", 1);

    local rb2 = cube2:add_rigid();
    rb2.mass = 10;
    rb2.bounciness = 0.3;
    rb2.type = BodyType.Dynamic;
    cube2:add_box_collider(Vector3.new(1.0, 1.0, 1.0));


    local sphere = scene:create_entity("Sphere", "");
    sphere:get_transform().pos = Vector3.new(10.0, 50.0, 0.0);
    sphere:get_transform().rot = Vector3.new(0.0, 0.0, 0.0);
    sphere:add_model("coreAssets/models/sphere.obj", 1);

    local rb1 = sphere:add_rigid();
    rb1.mass = 5;
    rb1.bounciness = 0.9;
    rb1.type = BodyType.Dynamic;
    sphere:add_sphere_collider(1.0);



    local sphere2 = scene:create_entity("Sphere 2", "");
    sphere2:get_transform().pos = Vector3.new(10.0, 70.0, 0.0);
    sphere2:get_transform().rot = Vector3.new(0.0, 0.0, 0.0);
    sphere2:add_model("coreAssets/models/sphere.obj", 1);

    local rb3 = sphere2:add_rigid();
    rb3.mass = 5;
    rb3.bounciness = 0.9;
    rb3.type = BodyType.Dynamic;
    sphere2:add_sphere_collider(1.0);

end


function phys_objs()
    local sphere = scene:create_entity("Sphere", "");
    sphere:get_transform().pos = Vector3.new(60.0, 22.0, 0.0);
    sphere:get_transform().rot = Vector3.new(0.0, 0.0, 0.0);
    sphere:add_model("coreAssets/models/sphere.obj", 1);

    local rb1 = sphere:add_rigid();
    rb1.mass = 5;
    rb1.bounciness = 0.3;
    rb1.type = BodyType.Dynamic;
    rb1.hasGravity = false;
    sphere:add_sphere_collider(1.0);

    local wall1 = scene:create_entity("Wall 1", "");
    wall1:get_transform().pos = Vector3.new(50.0, 20.0, 0.0);
    wall1:get_transform().rot = Vector3.new(90.0, 90.0, 0.0);
    wall1:get_transform().scale = Vector3.new(4.0, 0.5, 4.0);
    wall1:add_model("coreAssets/models/cube.obj", 1);

    local rb2 = wall1:add_rigid();
    rb2.mass = 1;
    rb2.bounciness = 0.3;
    rb2.type = BodyType.Dynamic;
    rb2.hasGravity = false;
    wall1:add_box_collider(Vector3.new(4.0, 0.25, 4.0));

    local wall2 = scene:create_entity("wall 2", "");
    wall2:get_transform().pos = Vector3.new(40.0, 20.0, 0.0);
    wall2:get_transform().rot = Vector3.new(90.0, 90.0, 0.0);
    wall2:get_transform().scale = Vector3.new(4.0, 0.5, 4.0);
    wall2:add_model("coreAssets/models/cube.obj", 1);

    local rb3 = wall2:add_rigid();
    rb3.mass = 1;
    rb3.bounciness = 0.3;
    rb3.type = BodyType.Dynamic;
    rb3.hasGravity = false;
    wall2:add_box_collider(Vector3.new(4.0, 0.25, 4.0));
end

function update()

    if (Input:get_key_down(KEYS["KEY_ESC"])) then
        change_scene("test menu");
    end
    
    GUI:start_window("Stats", false, false, 0.0, 0.0 , 25, 25); 

    GUI:add_text("GAME FPS: " .. Time.fps, false, false, 1.0, 0.0, 1.0, 0.0, 1.0);

    GUI:end_window();
end

function tick()
end