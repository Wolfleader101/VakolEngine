function init(scene, entity)
    print("Initialising Terrain");

    local terrain = entity:create_height_map_terrain("coreAssets/textures/Heightmaps/height128.raw"); -- size

    entity:get_transform().pos.y =  entity:get_transform().pos.y - 41;
    entity:get_transform().scale.x = 1;
    entity:get_transform().scale.y = 1;
    entity:get_transform().scale.z = 1;

    

    local model = terrain:get_model();
    

    model:set_shader("coreAssets/shaders/basic.prog") -- set the shader on the model (automatically binds the shader)
    
    
    scene:add_terrain_physics(entity);
    
    
end

function update(scene, entity)

end