function init()
    print("Initialising Terrain");
  
    local terrain = entity:create_height_map_terrain("coreAssets/textures/Heightmaps/height128.raw", 0, 30); -- size

    local model = terrain:get_model();
    local mesh = model:get_mesh(0);
    local material = mesh:get_material();

    entity:add_raw_texture(0, "coreAssets/textures/lightmap.raw");

    entity:add_texture(0, "coreAssets/textures/Terrain/dirt_0.jpg", false, false);
    entity:add_texture(0, "coreAssets/textures/Terrain/dirt_3.jpg", false, false);
    entity:add_texture(0, "coreAssets/textures/Terrain/dirt_cracked.jpg", false, false);
    entity:add_texture(0, "coreAssets/textures/Terrain/patch_grass_1.jpg", false, false);
    entity:add_texture(0, "coreAssets/textures/Terrain/grass_0.png", false, false);
    entity:add_texture(0, "coreAssets/textures/Terrain/grass_rock.jpg", false, false);
    entity:add_texture(0, "coreAssets/textures/Terrain/rocks_0.jpg", false, false);
    entity:add_texture(0, "coreAssets/textures/Terrain/snow_cracks.jpg", false, false);
    entity:add_texture(0, "coreAssets/textures/Terrain/snow_0.jpg", false, false);
    entity:add_texture(0, "coreAssets/textures/Terrain/snow.png", false, false);

    entity:set_shader("coreAssets/shaders/basic_terrain.prog");
    local shader = model:get_shader();

    shader:set_vec2v("uv_scale", Vector2.new(10));

    shader:set_int("light_map", 0);

    shader:set_int("layer_1", 1);
    shader:set_int("layer_2", 2);
    shader:set_int("layer_3", 3);
    shader:set_int("layer_4", 4);
    shader:set_int("layer_5", 5);
    shader:set_int("layer_6", 6);
    shader:set_int("layer_7", 7);
    shader:set_int("layer_8", 8);
    shader:set_int("layer_9", 9);
    shader:set_int("layer_10", 10);

    scene:add_terrain_physics(entity);

    scene.globals.terrain = {transform = entity:get_transform(), terr = entity:get_terrain()};
end

function update()
    
end