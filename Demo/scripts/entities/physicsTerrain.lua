function init(scene, entity)
    print("Initialising Terrain");
  
    local terrain = entity:create_height_map_terrain("coreAssets/textures/Heightmaps/height128.raw", 0, 1); -- size

    entity:get_transform().pos = Vector3.new(1.1, -0.75, 1.1);
    entity:get_transform().scale = Vector3.new(1, 1, 1);

    local model = terrain:get_model();
    

    model:set_shader("coreAssets/shaders/basic.prog") -- set the shader on the model (automatically binds the shader)
    
    
    scene:add_terrain_physics(entity);
    
    
end

function update(scene, entity)

    
    
end