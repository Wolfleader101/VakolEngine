function init()
    print("Initialising Terrain");
    local terrain = entity:create_height_map_terrain("coreAssets/textures/Heightmaps/height128.raw", 0, 30);
    
    local layer_1 = create_texture("coreAssets/textures/extras/clover.png", false, false);
    local layer_2 = create_texture("coreAssets/textures/extras/grass.png", false, false);
    local layer_3 = create_texture("coreAssets/textures/extras/grass_dark.png", false, false);
    local layer_4 = create_texture("coreAssets/textures/extras/rocks_2.jpg", false, false);
    local layer_5 = create_texture("coreAssets/textures/extras/rocks.jpg", false, false);
    local layer_6 = create_texture("coreAssets/textures/extras/snow.png", false, false);

    local model = terrain:get_model();
    local mesh = model:get_mesh(0);
    local material = mesh:get_material();

    material:add_texture(layer_1);
    material:add_texture(layer_2);
    material:add_texture(layer_3);
    material:add_texture(layer_4);
    material:add_texture(layer_5);
    material:add_texture(layer_6);

    model:set_shader("coreAssets/shaders/basic_terrain.prog");
    local shader = model:get_shader();

    shader:set_vec2v("uv_scale", Vector2.new(20));

    shader:set_int("layer_1", 0);
    shader:set_int("layer_2", 1);
    shader:set_int("layer_3", 2);
    shader:set_int("layer_4", 3);
    shader:set_int("layer_5", 4);
    shader:set_int("layer_6", 5);
end

function update()

end