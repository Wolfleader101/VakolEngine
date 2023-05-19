function init(scene, entity)
    print("Initialising Terrain");
    local terrain = entity:create_height_map_terrain("coreAssets/textures/Heightmaps/height128.raw", 0, 1);
    local diffuse_map = create_texture("coreAssets/textures/grass.png", false, false);

    local model = terrain:get_model();
    local mesh = model:get_mesh(0);
    local material = mesh:get_material();

    material:add_texture(diffuse_map);

    model:set_shader("coreAssets/shaders/basic_terrain.prog");
    local shader = model:get_shader();

    shader:set_int("diffuse_map", 0);
end

function update(scene, entity)

end