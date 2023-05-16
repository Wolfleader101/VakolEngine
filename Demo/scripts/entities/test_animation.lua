function init(scene, entity)

    local IDLE_STATE = 0;
    local WALK_STATE = 1;
    local RUN_STATE = 2;
    
    entity:get_transform().pos = Vector3.new(0.0, 0.0, -4.0);
    entity:get_transform().rot = Vector3.new(0.0, 85.0, 0.0);

    local model = entity:add_model("assets/models/robot_dance.fbx", 1.0, true) -- get model and add a drawable component
    model:set_shader("coreAssets/shaders/animation.prog") -- set the shader on the model (automatically binds it)

    model:set_animation_state(0);

    local shader = model:get_shader(); -- get the shader from the model

    shader:set_vec3v("light.position", Vector3.new(0.0, 2.0, -4.0));
    shader:set_vec3v("light.direction", Vector3.new(math.rad(0.0), math.rad(0.0), math.rad(90.0))); -- light not working right at the moment

    shader:set_float("material.shininess", 32.0);

    shader:set_int("material.diffuse_map", 0);
    shader:set_int("material.specular_map", 1);
    shader:set_int("material.normal_map", 2);
    shader:set_int("material.emission_map", 3);
end

function update(scene, entity)
    
end