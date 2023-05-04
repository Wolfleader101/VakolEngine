function init(scene, entity)
    print("Initialising Terrain");
    local terrain = entity:add_clod_terrain("coreAssets/textures/Heightmaps/height128.raw"); -- size

    local hmap = raw_texture("coreAssets/textures/Heightmaps/height128.raw");
    hmap:bind_texture(0);
    
    local model = terrain:get_model();
    model:set_shader("coreAssets/shaders/clod_terrain.prog");
    
    local shader = model:get_shader();
    shader:set_int("height_map", 0);
end

function update(scene, entity)

end