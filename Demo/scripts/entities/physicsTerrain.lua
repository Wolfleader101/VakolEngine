function init(scene, entity)
    print("Initialising Terrain");
    local terrain = entity:add_heightmap_terrain("coreAssets/textures/HeightMaps/height128.raw"); -- size

    entity:get_transform().pos.y = -100;
    entity:get_transform().scale.x = 1;
    entity:get_transform().scale.y = 1;
    entity:get_transform().scale.z = 1;

    local model = terrain:get_model();
    

    model:set_shader("coreAssets/shaders/basic.prog") -- set the shader on the model (automatically binds the shader)
    


    
end

function update(scene, entity)

end