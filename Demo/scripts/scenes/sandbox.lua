function init()
    scene:create_entity("Player", "entities/player/noclip.lua")
    --scene:create_entity("Player", "entities/player/phys_player.lua")

    scene:create_entity("Digital Twin", "entities/worlds/digital_twin.lua");
    scene:create_entity("290 World", "entities/worlds/290_world.lua");

    -- AI

    scene.globals.emotional_entities = {};

    local shri = scene:create_entity("shri", "entities/agents/shri_rai.lua");
    shri:get_transform().pos = Vector3.new(-20, 5, -20);
    shri:get_transform().scale = Vector3.new(0.015, 0.015, 0.015);
    scene.globals.emotional_entities[1] = shri;

    local hong = scene:create_entity("Hong", "entities/agents/hong.lua");
    scene.globals.emotional_entities[2] = hong;

    local kiki = scene:create_entity("Kiki", "entities/agents/kiki.lua");
    scene.globals.emotional_entities[1] = kiki;

    -- local test = scene:create_entity("bob", "");
    -- test:add_model("assets/models/agents/bob.glb", 0.01);
    -- shri:get_transform().pos = Vector3.new(-8, 5, -9);

    local path = "coreAssets/textures/Skybox/";
    local extension = ".png";

    local faces = { path .. "right" .. extension, path .. "left" .. extension, path .. "top" .. extension,
        path .. "bottom" .. extension, path .. "front" .. extension, path .. "back" .. extension };

    scene:generate_skybox(faces);

    --add_floor();

    -- phys_objs();
    -- falling_objs();
    -- stress_test();
    -- ramps();
    -- particles();

    -- local cube = scene:create_entity("Cube", "");
    -- cube:get_transform().pos = Vector3.new(10.0, 20.0, 0.0);
    -- cube:get_transform().rot = Vector3.new(0.0, 0.0, 90.0);
    -- cube:get_transform().scale = Vector3.new(2, 1, 1);
    -- cube:add_model("coreAssets/models/cube.obj", 1);

    -- local rb = cube:add_rigid();
    -- rb.mass = 100;
    -- rb.bounciness = 0.3;
    -- rb.type = BodyType.Dynamic;
    -- cube:add_box_collider(Vector3.new(1, 0.5, 0.5));

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

function add_floor()
    local floor = scene:create_entity("Floor", "");
    floor:get_transform().scale = Vector3.new(100.0, 1.0, 100.0);
    local mdl = floor:add_model("coreAssets/models/cube.obj", 1);
    mdl:get_mesh(0).material:set_light_position(Vector3.new(0.0, 100.0, 0.0));

    local rb1 = floor:add_rigid();
    rb1.mass = 1;
    rb1.bounciness = 0.4;
    rb1.type = BodyType.Static;
    floor:add_box_collider(floor:get_transform().scale / 2);
end

function randomFloat(lower, greater)
    return lower + math.random() * (greater - lower);
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
    cube:add_box_collider(Vector3.new(0.5, 0.5, 0.5));

    local cube2 = scene:create_entity("Cube2", "");
    cube2:get_transform().pos = Vector3.new(10, 5.0, 0.0);
    cube2:get_transform().rot = Vector3.new(0.0, 0.0, 0.0);
    cube2:add_model("coreAssets/models/cube.obj", 1);

    local rb2 = cube2:add_rigid();
    rb2.mass = 10;
    rb2.bounciness = 0.3;
    rb2.type = BodyType.Dynamic;
    cube2:add_box_collider(Vector3.new(0.5, 0.5, 0.5));

    local sphere = scene:create_entity("Sphere", "");
    sphere:get_transform().pos = Vector3.new(10.0, 50.0, 0.0);
    sphere:get_transform().rot = Vector3.new(0.0, 0.0, 0.0);
    sphere:add_model("coreAssets/models/sphere.obj", 1);

    local rb1 = sphere:add_rigid();
    rb1.mass = 5;
    rb1.bounciness = 0.9;
    rb1.type = BodyType.Dynamic;
    sphere:add_sphere_collider(0.5);

    local sphere2 = scene:create_entity("Sphere 2", "");
    sphere2:get_transform().pos = Vector3.new(10.0, 70.0, 0.0);
    sphere2:get_transform().rot = Vector3.new(0.0, 0.0, 0.0);
    sphere2:add_model("coreAssets/models/sphere.obj", 1);

    local rb3 = sphere2:add_rigid();
    rb3.mass = 5;
    rb3.bounciness = 0.9;
    rb3.type = BodyType.Dynamic;
    sphere2:add_sphere_collider(0.5);
end

local wallRb = nil;
local sphereRb = nil;

function phys_objs()
    local sphere = scene:create_entity("Sphere", "");
    sphere:get_transform().pos = Vector3.new(60.0, 21.0, 0.0);
    sphere:get_transform().rot = Vector3.new(0.0, 0.0, 0.0);
    local mdl1 = sphere:add_model("coreAssets/models/sphere.obj", 1);
    local mesh1 = mdl1:get_mesh(0);

    mesh1.material:set_diffuse_color(Vector4.new(randomFloat(0, 1), randomFloat(0, 1), randomFloat(0, 1), 1.0));

    sphereRb = sphere:add_rigid();
    sphereRb.mass = 1;
    sphereRb.bounciness = 0.3;
    sphereRb.type = BodyType.Dynamic;
    sphereRb.hasGravity = false;
    sphere:add_sphere_collider(0.5);

    local wall1 = scene:create_entity("Wall 1", "");
    wall1:get_transform().pos = Vector3.new(50.0, 20.0, 0.0);
    wall1:get_transform().rot = Vector3.new(90.0, 90.0, 0.0);
    wall1:get_transform().scale = Vector3.new(4.0, 0.5, 4.0);
    local mdl2 = wall1:add_model("coreAssets/models/cube.obj", 1);
    local mesh2 = mdl2:get_mesh(0);

    mesh2.material:set_diffuse_color(Vector4.new(randomFloat(0, 1), randomFloat(0, 1), randomFloat(0, 1), 1.0));

    local rb2 = wall1:add_rigid();
    rb2.mass = 2;
    rb2.bounciness = 0.3;
    rb2.type = BodyType.Dynamic;
    rb2.hasGravity = false;
    wall1:add_box_collider(wall1:get_transform().scale / 2);
    wallRb = rb2;

    local wall2 = scene:create_entity("wall 2", "");
    wall2:get_transform().pos = Vector3.new(40.0, 20.0, 0.0);
    wall2:get_transform().rot = Vector3.new(90.0, 90.0, 0.0);
    wall2:get_transform().scale = Vector3.new(4.0, 0.5, 4.0);
    local mdl3 = wall2:add_model("coreAssets/models/cube.obj", 1);
    local mesh3 = mdl3:get_mesh(0);

    mesh3.material:set_diffuse_color(Vector4.new(randomFloat(0, 1), randomFloat(0, 1), randomFloat(0, 1), 1.0));

    local rb3 = wall2:add_rigid();
    rb3.mass = 1;
    rb3.bounciness = 0.3;
    rb3.type = BodyType.Dynamic;
    rb3.hasGravity = false;
    wall2:add_box_collider(wall2:get_transform().scale / 2);
end

function stress_test()
    for i = 1, 1000 do
        local entityName = "Cube" .. " " .. i;

        local cube = scene:create_entity(entityName, "");
        cube:get_transform().pos = Vector3.new(randomFloat(-50, 50), randomFloat(10, 50), randomFloat(-50, 50));
        cube:get_transform().rot = Vector3.new(randomFloat(-365, 365), randomFloat(-365, 365), randomFloat(-365, 365));
        local mdl = cube:add_model("coreAssets/models/cube.obj", 1);

        local mesh = mdl:get_mesh(0);

        mesh.material:set_diffuse_color(Vector4.new(randomFloat(0, 1), randomFloat(0, 1), randomFloat(0, 1), 1.0));

        local rb = cube:add_rigid();
        rb.mass = randomFloat(5, 100);
        rb.bounciness = randomFloat(0, 1);
        rb.type = BodyType.Dynamic;
        cube:add_box_collider(cube:get_transform().scale / 2);
    end
end

function ramps()
    local ramp1 = scene:create_entity("Ramp 1", "");
    ramp1:get_transform().pos = Vector3.new(20.0, 200.0, 0.0);
    ramp1:get_transform().rot = Vector3.new(-35, 90.0, 0.0);
    ramp1:get_transform().scale = Vector3.new(75.0, 0.5, 75.0);
    ramp1:add_model("coreAssets/models/cube.obj", 1);

    local rb1 = ramp1:add_rigid();
    rb1.mass = 2;
    rb1.bounciness = 0.3;
    rb1.type = BodyType.Static;
    rb1.hasGravity = false;
    ramp1:add_box_collider(ramp1:get_transform().scale / 2);
    wallRb = rb1;

    local ramp2 = scene:create_entity("Ramp 2", "");
    ramp2:get_transform().pos = Vector3.new(2.0, 150.0, 0.0);
    ramp2:get_transform().rot = Vector3.new(15, 90.0, 0.0);
    ramp2:get_transform().scale = Vector3.new(75.0, 0.5, 75.0);
    ramp2:add_model("coreAssets/models/cube.obj", 1);

    local rb2 = ramp2:add_rigid();
    rb2.mass = 1;
    rb2.bounciness = 0.3;
    rb2.type = BodyType.Static;
    rb2.hasGravity = false;
    ramp2:add_box_collider(ramp2:get_transform().scale / 2);

    local ramp3 = scene:create_entity("Ramp 3", "");
    ramp3:get_transform().pos = Vector3.new(40.0, 90.0, 0.0);
    ramp3:get_transform().rot = Vector3.new(-45, 90.0, 0.0);
    ramp3:get_transform().scale = Vector3.new(75.0, 0.5, 45.0);
    ramp3:add_model("coreAssets/models/cube.obj", 1);

    local rb3 = ramp3:add_rigid();
    rb3.mass = 1;
    rb3.bounciness = 0.3;
    rb3.type = BodyType.Static;
    rb3.hasGravity = false;
    ramp3:add_box_collider(ramp3:get_transform().scale / 2);

    local ramp4 = scene:create_entity("Ramp 4", "");
    ramp4:get_transform().pos = Vector3.new(20.0, 50.0, 0.0);
    ramp4:get_transform().rot = Vector3.new(10, 90.0, 0.0);
    ramp4:get_transform().scale = Vector3.new(75.0, 0.5, 45.0);
    ramp4:add_model("coreAssets/models/cube.obj", 1);

    local rb4 = ramp4:add_rigid();
    rb4.mass = 1;
    rb4.bounciness = 0.3;
    rb4.type = BodyType.Static;
    rb4.hasGravity = false;
    ramp4:add_box_collider(ramp4:get_transform().scale / 2);

    for i = 1, 400 do
        local entityName = "Cube" .. " " .. i;

        local cube = scene:create_entity(entityName, "");
        cube:get_transform().pos = Vector3.new(randomFloat(-25, 50), randomFloat(220, 300), randomFloat(-15, 30));
        cube:get_transform().rot = Vector3.new(randomFloat(-365, 365), randomFloat(-365, 365), randomFloat(-365, 365));
        local mdl = cube:add_model("coreAssets/models/cube.obj", 1);

        local mesh = mdl:get_mesh(0);

        mesh.material:set_diffuse_color(Vector4.new(randomFloat(0, 1), randomFloat(0, 1), randomFloat(0, 1), 1.0));

        local rb = cube:add_rigid();
        rb.mass = randomFloat(5, 100);
        rb.bounciness = randomFloat(0, 1);
        rb.type = BodyType.Dynamic;
        cube:add_box_collider(cube:get_transform().scale / 2);
    end

    for i = 1, 400 do
        local entityName = "Sphere" .. " " .. i;

        local sphere = scene:create_entity(entityName, "");
        sphere:get_transform().pos = Vector3.new(randomFloat(-25, 50), randomFloat(220, 300), randomFloat(-15, 30));
        local mdl = sphere:add_model("coreAssets/models/sphere.obj", 1);

        local mesh = mdl:get_mesh(0);

        mesh.material:set_diffuse_color(Vector4.new(randomFloat(0, 1), randomFloat(0, 1), randomFloat(0, 1), 1.0));

        local rb = sphere:add_rigid();
        rb.mass = randomFloat(5, 100);
        rb.bounciness = randomFloat(0, 1);
        rb.type = BodyType.Dynamic;
        sphere:add_sphere_collider(0.5);
    end
end

function particles()
    local ramp1 = scene:create_entity("Ramp 1", "");
    ramp1:get_transform().pos = Vector3.new(20.0, 30.0, 0.0);
    ramp1:get_transform().rot = Vector3.new(-35, 90.0, 0.0);
    ramp1:get_transform().scale = Vector3.new(7.5, 0.05, 7.5);
    ramp1:add_model("coreAssets/models/cube.obj", 1);

    local rb1 = ramp1:add_rigid();
    rb1.mass = 2;
    rb1.bounciness = 0.5;
    rb1.type = BodyType.Static;
    rb1.hasGravity = false;
    ramp1:add_box_collider(ramp1:get_transform().scale / 2);
    wallRb = rb1;

    local ramp2 = scene:create_entity("Ramp 2", "");
    ramp2:get_transform().pos = Vector3.new(13.0, 23.0, 0.0);
    ramp2:get_transform().rot = Vector3.new(15, 90.0, 0.0);
    ramp2:get_transform().scale = Vector3.new(7.5, 0.05, 7.5);
    ramp2:add_model("coreAssets/models/cube.obj", 1);

    local rb2 = ramp2:add_rigid();
    rb2.mass = 1;
    rb2.bounciness = 0.5;
    rb2.type = BodyType.Static;
    rb2.hasGravity = false;
    ramp2:add_box_collider(ramp2:get_transform().scale / 2);

    local ramp3 = scene:create_entity("Ramp 3", "");
    ramp3:get_transform().pos = Vector3.new(22.0, 20.0, 0.0);
    ramp3:get_transform().rot = Vector3.new(-45, 90.0, 0.0);
    ramp3:get_transform().scale = Vector3.new(7.5, 0.05, 4.5);
    ramp3:add_model("coreAssets/models/cube.obj", 1);

    local rb3 = ramp3:add_rigid();
    rb3.mass = 1;
    rb3.bounciness = 0.7;
    rb3.type = BodyType.Static;
    rb3.hasGravity = false;
    ramp3:add_box_collider(ramp3:get_transform().scale / 2);

    for i = 1, 1000 do
        local entityName = "Sphere" .. " " .. i;

        local sphere = scene:create_entity(entityName, "");
        sphere:get_transform().pos = Vector3.new(randomFloat(20, 25), randomFloat(35, 40), randomFloat(-5, 5));
        sphere:get_transform().scale = Vector3.new(0.1, 0.1, 0.1);
        local mdl = sphere:add_model("coreAssets/models/sphere.obj", 1);

        local mesh = mdl:get_mesh(0);

        mesh.material:set_diffuse_color(Vector4.new(randomFloat(0, 1), randomFloat(0, 1), randomFloat(0, 1), 1.0));

        local rb = sphere:add_rigid();
        rb.mass = 1
        rb.bounciness = 0.7;
        rb.type = BodyType.Dynamic;
        sphere:add_sphere_collider(0.05);
    end
end

function update()
    if (Input:get_key_down(KEYS["KEY_ESC"])) then
        change_scene("test menu");
    end

    if (sphereRb ~= nil and Input:get_key_down(KEYS["KEY_1"])) then
        sphereRb:apply_impulse(Vector3.new(-75.0, 0.0, 00.0));
    end

    if (wallRb ~= nil and Input:get_key_down(KEYS["KEY_2"])) then
        wallRb:apply_angular_impulse(Vector3.new(0.0, 0.0, 5.0), Vector3.new(0.0, 1.0, 0.0));
    end

    if (Input:get_key_down(KEYS["KEY_P"])) then
        scene:serialize("assets/scenes/" .. scene:get_name());
    end

    if (Input:get_key_down(KEYS["KEY_O"])) then
        scene:deserialize("assets/scenes/" .. scene:get_name());
    end

    GUI:start_window("Stats", false, false, 0.0, 0.0, 25, 25);

    GUI:add_text("GAME FPS: " .. Time.fps, false, false, 1.0, 0.0, 1.0, 0.0, 1.0);

    GUI:end_window();
end

function tick()
end
