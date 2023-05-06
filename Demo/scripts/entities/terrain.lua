function init(scene, entity)
    print("Initialising Terrain");
    -- Terrain = entity:create_height_map_terrain("coreAssets/textures/Heightmaps/height128.raw");
    Terrain = entity:create_clod_terrain("coreAssets/textures/Heightmaps/height128.raw");
    local height_map = create_raw_texture("coreAssets/textures/Heightmaps/height128.raw");

    local model = Terrain:get_model();

    --model:set_shader("coreAssets/shaders/basic_terrain.prog");
    model:set_shader("coreAssets/shaders/clod_terrain.prog");

    local mesh = model:get_mesh(0);
    local shader = model:get_shader();
    local material = mesh:get_material();

    material:add_texture(height_map);

    shader:set_int("height_map", 0);
end

function update(scene, entity)

end