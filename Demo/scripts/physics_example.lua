function init(scene, entity)

    --local ent = scene:create_entity("basicEntity.lua");
    local model = entity:add_model("coreAssets/models/cube.obj");

    model:set_shader("coreAssets/shaders/basic.prog");
    
    local rigid = entity:add_rigid();
    
    --local trans = entity:get_transform();
    --print("sdf");
    

    entity:physics_init(scene);
    print("sdf");

end

function update(scene, entity)
    
end

