function init(scene, entity)
    print("Initialising Terrain");
    local terrain = entity:add_fault_formation_terrain(512, 256, 0.7, true, 0, 50); -- size

    entity:get_transform().pos.y =  entity:get_transform().pos.y - 0.1;
    entity:get_transform().scale.x = 1;
    entity:get_transform().scale.y = 1;
    entity:get_transform().scale.z = 1;

    local model = terrain:get_model();
    

    model:set_shader("coreAssets/shaders/basic.prog") -- set the shader on the model (automatically binds the shader)
    

    scene:add_terrain_physics(entity);
    
end

function update(scene, entity)

end