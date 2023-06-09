function init()
    print("Initialising Terrain");
  
    local terrain = entity:create_height_map_terrain(scene, "assets/textures/Heightmaps/terrain_2048.raw", 0, 60); -- size
    --local terrain = entity:create_height_map_terrain(scene, "assets/textures/Heightmaps/height128.raw", 0, 1); -- size

    local model = terrain:get_model();
    local mesh = model:get_mesh(0);
    local material = mesh:get_material();

    entity:add_raw_texture(0, "assets/textures/lightmap.raw");

    entity:add_texture(0, "assets/textures/terrain/TEX_128_DirtWithRocks.jpg", false, false);
    entity:add_texture(0, "assets/textures/terrain/TEX_128_DirtNoRocks.jpg", false, false);
    entity:add_texture(0, "assets/textures/terrain/TEX_128_Grass.jpg", false, false);
    entity:add_texture(0, "assets/textures/terrain/TEX_128_SnowNoRocks.jpg", false, false);
    entity:add_texture(0, "assets/textures/terrain/TEX_128_SnowWithRocks.jpg", false, false);

    entity:add_texture(0, "assets/textures/noise/TEX_1024_Noise.jpg", false, false);

    entity:add_texture(0, "assets/textures/Water/water_1.jpg", false, false);
    entity:add_texture(0, "assets/textures/Water/water_4.png", false, false);

    entity:set_shader("coreAssets/shaders/basic_terrain.prog");
    shader = model:get_shader();

    shader:set_vec2v("uv_scale", Vector2.new(20));

    shader:set_int("light_map", 0);

    shader:set_int("layer_1", 1);
    shader:set_int("layer_2", 2);
    shader:set_int("layer_3", 3);
    shader:set_int("layer_4", 4);
    shader:set_int("layer_5", 5);

    shader:set_int("noise", 6);

    shader:set_int("water_layer_1", 7);
    shader:set_int("water_layer_2", 8);

    -- scene:add_terrain_physics(entity);

    scene.globals.terrain = { transform = entity:get_transform(), terr = entity:get_terrain() };

    local planeEntity = scene:create_entity("planeEntity", "")
    local planeModel = planeEntity:add_model("coreAssets/models/plane.obj", scene.globals.terrain.terr:get_size(), false, true) -- get model and add a drawable component
    
    local trans = planeEntity:get_transform();

    trans.pos = Vector3.new(0.0, scene.globals.waterlevel - 10.74, 0.0);
    
    planeEntity:set_shader("coreAssets/shaders/water.prog") -- set the shader on the model (automatically binds it)

    plane_shader = planeModel:get_shader();

    entity:add_texture(0, "assets/textures/Water/water_1.jpg", false, false);
    entity:add_texture(0, "assets/textures/Water/water_4.png", false, false);

    plane_shader:set_int("texture_0", 9);
    plane_shader:set_int("texture_1", 10);
end

function update()
    shader:set_float("time", Time.curr_time);

    plane_shader:set_float("time", Time.curr_time);
end

function deserialize()
    scene.globals.terrain = { transform = entity:get_transform(), terr = entity:get_terrain() };
end