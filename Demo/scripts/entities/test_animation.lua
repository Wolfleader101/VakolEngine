function init(scene, entity)
    
    entity:get_transform().pos = Vector3.new(-10.0, 0.0, 10.0);
    entity:get_transform().rot = Vector3.new(-90.0, 0.0, 90.0);  

    local model = entity:add_model("assets/models/monsters/soldier/tris.md2") -- get model and add a drawable component
    model:set_shader("coreAssets/shaders/basic.prog") -- set the shader on the model (automatically binds it)
    
    local shader = model:get_shader(); -- get the shader from the model
    shader:set_int("diffuse_map", 0);
end

function update(scene, entity)
    
end