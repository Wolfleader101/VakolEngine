function init(scene, entity)
    print("Initialising Terrain");
    local terrain = entity:add_clod_terrain("coreAssets/textures/Heightmaps/height128.raw"); -- size

    local model = terrain:get_model();
    model:set_shader("coreAssets/shaders/clod_terrain.prog");
end

function update(scene, entity)

end