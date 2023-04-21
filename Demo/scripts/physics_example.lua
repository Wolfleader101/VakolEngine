function init(scene, entity)

    --local ent = scene:create_entity("basicEntity.lua");
    local model = entity:add_model("coreAssets/models/cube.obj");

    model:set_shader("coreAssets/shaders/basic.prog");



    -- local SP = scene:get_physics();
    
    -- local rigid = entity:add_rigidbody(SP);
    
    -- local trans = entity:get_transform();
    -- print("sdf");
    

    -- SP:init_object(rigid, trans, model);

end

function update(scene, entity)
    
end

