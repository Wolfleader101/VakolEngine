local function random_pos()
    -- Constants for the sphere
    local radius = 100

    -- Generate random spherical coordinates
    local u = math.random()
    local v = math.random()
    local theta = 2 * math.pi * u
    local phi = math.acos(2 * v - 1)

    -- Convert spherical coordinates to Cartesian coordinates
    local x = radius * math.sin(phi) * math.cos(theta)
    local z = radius * math.cos(phi)

    return Vector3.new(x, 0, z)
end


function init()
    scene.globals.waterlevel = 1.0;

    scene:create_entity("Player", "entities/player/player.lua")

    scene:create_entity("terrain", "entities/terrain/physicsTerrain.lua");

    scene:create_entity("serialization", "entities/misc/serialization.lua");

    scene:create_entity("static_props", "entities/instancing_new.lua");

    local rabbits          = {};
    local birds            = {};
    local deers            = {};
    scene.globals.monsters = {};
    scene.globals.bears    = {};

    for i = 1, 10 do
        scene.globals.bears[i] = scene:create_entity("Bear" .. i, "entities/agents/bear.lua");
        scene.globals.bears[i]:get_transform().pos = random_pos();

        local rb = scene.globals.bears[i]:add_rigid();

        rb.use_transform = true;

        local collider = scene.globals.bears[i]:add_collider();

        collider.Shape = Shape.Box;
        collider.bounds.extents.x = 0.1;
        collider.bounds.extents.y = 0.5;
        collider.bounds.extents.z = 0.2;


        scene.globals.bears[i]:physics_init(scene);
    
    end

    for i = 1, 10 do
        scene.globals.monsters[i] = scene:create_entity("Monster" .. i, "entities/agents/monster.lua");
        scene.globals.monsters[i]:get_transform().pos = random_pos();

        local rb = scene.globals.monsters[i]:add_rigid();

        rb.use_transform = true;

        local collider = scene.globals.monsters[i]:add_collider();

        collider.Shape = Shape.Box;
        collider.bounds.extents.x = 0.1;
        collider.bounds.extents.y = 0.5;
        collider.bounds.extents.z = 0.1;

       scene.globals.monsters[i]:physics_init(scene);
    end

    for i = 1,10 do
        birds[i] = scene:create_entity("Bird" .. i, "entities/agents/bird.lua");
        birds[i]:get_transform().pos = random_pos();
    end

    for i = 1, 10 do
        rabbits[i] = scene:create_entity("Rabbit" .. i, "entities/agents/rabbit.lua");
        rabbits[i]:get_transform().pos = random_pos();

        local rb =  rabbits[i]:add_rigid();

        rb.use_transform = true;

        local collider =  rabbits[i]:add_collider();

        collider.Shape = Shape.Box;
        collider.bounds.extents.x = 0.05;
        collider.bounds.extents.y = 0.1;
        collider.bounds.extents.z = 0.05;

        rabbits[i]:physics_init(scene);
    end

    for i = 1, 5 do
        deers[i] = scene:create_entity("Deer" .. i, "entities/agents/deer.lua");
        deers[i]:get_transform().pos = random_pos();

        local rb = deers[i]:add_rigid();

        rb.use_transform = true;

        local collider = deers[i]:add_collider();

        collider.Shape = Shape.Box;
        collider.bounds.extents.x = 0.1;
        collider.bounds.extents.y = 0.5;
        collider.bounds.extents.z = 0.2;


        deers[i]:physics_init(scene);
    end

    phys_debug = false;
end


function update()
    if (Input:get_key_down(KEYS["KEY_ESC"])) then
        toggle_wireframe();
        toggle_skybox();

        scene:set_active(false);

        local menu = get_scene("Start Scene");
        menu:set_active(true);
    end
    
    if (Input:get_key_down(KEYS["KEY_N"])) then
        phys_debug = not phys_debug;
        scene:enable_debug(phys_debug);
    end

    if(Input:get_key_down(KEYS["KEY_5"])) then
        scene:serialize("assets/scenes");
        print("Checkpoint saved!");
    end

    if(Input:get_key_down(KEYS["KEY_6"])) then
        scene:deserialize("assets/scenes/".. scene:get_name());
        print("Checkpoint loaded!")
    end
end