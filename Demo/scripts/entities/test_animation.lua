function init(scene, entity)
    
    entity:get_transform().pos = Vector3.new(-10.0, 0.0, 10.0);
    entity:get_transform().rot = Vector3.new(0.0, 85.0, 0.0);

    local model = entity:add_model("assets/models/backflip_2.dae", 0.1, true) -- get model and add a drawable component
    model:set_shader("coreAssets/shaders/animation.prog") -- set the shader on the model (automatically binds it)
    
    local shader = model:get_shader(); -- get the shader from the model

    shader:set_int("diffuse_map", 0);
    shader:set_int("specular_map", 1);
    shader:set_int("normal_map", 2);
    shader:set_int("emission_map", 3);
end

function update(scene, entity)
    
end