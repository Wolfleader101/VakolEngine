function init(scene, entity)
    print("Hello World");

    
    --local ent = scene:create_entity("basicEntity.lua");
    local model = entity:add_model("coreAssets/models/cube.obj");

    model:set_shader("coreAssets/shaders/basic.prog");

end

function update(scene, entity)
    
end

