function init(scene, entity)
    print("Initialising Terrain");

    Terrain = entity:create_height_map_terrain("coreAssets/textures/Heightmaps/height128.raw");


    local model = Terrain:get_model();
    model:set_shader("coreAssets/shaders/basic_terrain.prog");
end

function update(scene, entity)

end