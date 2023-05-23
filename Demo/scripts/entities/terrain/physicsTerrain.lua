function init()
    print("Initialising Terrain");
  
    local terrain = entity:create_height_map_terrain("coreAssets/textures/Heightmaps/terrain2.raw", 0, 60); -- size

    local lightmap = create_raw_texture("coreAssets/textures/lightmap.raw");
    
    local layer_1 = create_texture("coreAssets/textures/Terrain/dirt_0.jpg", false, false);
    local layer_2 = create_texture("coreAssets/textures/Terrain/dirt_3.jpg", false, false);
    local layer_3 = create_texture("coreAssets/textures/Terrain/dirt_cracked.jpg", false, false);
    local layer_4 = create_texture("coreAssets/textures/Terrain/patch_grass_1.jpg", false, false);
    local layer_5 = create_texture("coreAssets/textures/Terrain/grass_0.png", false, false);
    local layer_6 = create_texture("coreAssets/textures/Terrain/grass_rock.jpg", false, false);
    local layer_7 = create_texture("coreAssets/textures/Terrain/rocks_0.jpg", false, false);
    local layer_8 = create_texture("coreAssets/textures/Terrain/snow_cracks.jpg", false, false);
    local layer_9 = create_texture("coreAssets/textures/Terrain/snow_0.jpg", false, false);
    local layer_10 = create_texture("coreAssets/textures/Terrain/snow.png", false, false);

    local model = terrain:get_model();
    local mesh = model:get_mesh(0);
    local material = mesh:get_material();

    material:add_texture(lightmap);

    material:add_texture(layer_1);
    material:add_texture(layer_2);
    material:add_texture(layer_3);
    material:add_texture(layer_4);
    material:add_texture(layer_5);
    material:add_texture(layer_6);
    material:add_texture(layer_7);
    material:add_texture(layer_8);
    material:add_texture(layer_9);
    material:add_texture(layer_10);

    model:set_shader("coreAssets/shaders/basic_terrain.prog");
    local shader = model:get_shader();

    shader:set_bool("enable_fog", false);
    shader:set_float("FOG_DENSITY", 0.025);
    shader:set_vec4v("FOG_COLOR", Vector4.new(1.0, 1.0, 1.0, 0.0));

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