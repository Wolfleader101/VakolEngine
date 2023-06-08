function init()
    print("Initialising Terrain");
  
    local terrain = entity:create_height_map_terrain(scene, "coreAssets/textures/Heightmaps/terrain_2048.raw", -5, 35); -- size

    local model = terrain:get_model();
    local mesh = model:get_mesh(0);
    local material = mesh:get_material();

    entity:add_raw_texture(0, "coreAssets/textures/lightmap.raw");

    entity:add_texture(0, "assets/textures/terrain/TEX_128_DirtWithRocks.jpg", false, false);
    entity:add_texture(0, "assets/textures/terrain/TEX_128_DirtNoRocks.jpg", false, false);
    entity:add_texture(0, "assets/textures/terrain/TEX_128_Grass.jpg", false, false);
    entity:add_texture(0, "assets/textures/terrain/TEX_128_SnowNoRocks.jpg", false, false);
    entity:add_texture(0, "assets/textures/terrain/TEX_128_SnowWithRocks.jpg", false, false);

    entity:add_texture(0, "assets/textures/noise/TEX_1024_Noise.jpg", false, false);

    entity:set_shader("coreAssets/shaders/basic_terrain.prog");
    local shader = model:get_shader();

    shader:set_vec2v("uv_scale", Vector2.new(20));

    shader:set_int("light_map", 0);

    shader:set_int("layer_1", 1);
    shader:set_int("layer_2", 2);
    shader:set_int("layer_3", 3);
    shader:set_int("layer_4", 4);
    shader:set_int("layer_5", 5);

    shader:set_int("noise", 6);

    scene:add_terrain_physics(entity);

    scene.globals.terrain = {transform = entity:get_transform(), terr = entity:get_terrain()};
end

function update()
    
end