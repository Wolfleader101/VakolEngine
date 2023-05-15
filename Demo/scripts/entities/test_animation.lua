function init(scene, entity)
    
    entity:get_transform().pos = Vector3.new(0.0, 0.0, -4.0);
    entity:get_transform().rot = Vector3.new(0.0, 85.0, 0.0);

    local model = entity:add_model("assets/models/robot_dance.fbx", 1.0, true) -- get model and add a drawable component
    model:set_shader("coreAssets/shaders/animation.prog") -- set the shader on the model (automatically binds it)

    local mesh_0 = model:get_mesh(0);
    local mesh_1 = model:get_mesh(1);

    local material = mesh_0:get_material();

    local diffuse = material:get_diffuse_color();

    local shader = model:get_shader(); -- get the shader from the model

    shader:set_vec3v("tint", diffuse);

    shader:set_int("diffuse_map", 0);
    shader:set_int("specular_map", 1);
    shader:set_int("normal_map", 2);
    shader:set_int("emission_map", 3);
end

function update(scene, entity)
    
end