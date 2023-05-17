function init()
    print("Initialising Terrain");

    local terrain = entity:create_height_map_terrain("coreAssets/textures/Heightmaps/heightmap_custom.raw", 0, 255); -- size

    -- entity:get_transform().pos.y = -0.75;
    -- entity:get_transform().pos.z = 1.0;
    -- entity:get_transform().pos.x = 1.0;
    entity:get_transform().scale.x = 1;
    entity:get_transform().scale.y = 1;
    entity:get_transform().scale.z = 1;

    
    local model = terrain:get_model();
    
    model:set_shader("coreAssets/shaders/basic_terrain.prog") -- set the shader on the model (automatically binds the shader)

    scene:add_terrain_physics(entity);
end

function update()

    
    
end