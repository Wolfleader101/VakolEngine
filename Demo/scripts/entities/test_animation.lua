function init()

    entity:get_transform().pos = Vector3.new(0.0, 0.0, -5.0);

    local pos = entity:get_transform().pos;
    local terr_scale = scene.globals.terrain.transform.scale;
    pos.y = (scene.globals.terrain.terr:get_height(pos.x / terr_scale.x, pos.z / terr_scale.z) * terr_scale.y) + 0.05;

    entity:get_transform().rot = Vector3.new(0.0, 0.0, 0.0);

    state.model = entity:add_model("assets/models/test.fbx", 1.0, true, true) -- get model and add a drawable component
    entity:set_shader("coreAssets/shaders/animation.prog") -- set the shader on the model (automatically binds it)

    local shader = state.model:get_shader(); -- get the shader from the model

    shader:set_vec3v("light.direction", Vector3.new(math.rad(0.0), math.rad(0.0), math.rad(90.0)));

    shader:set_float("material.shininess", 16.0);
    shader:set_vec3v("tint", Vector3.new(0.2, 0.65, 0.9));

    shader:set_int("material.diffuse_map", 0);
    shader:set_int("material.specular_map", 1);
    shader:set_int("material.normal_map", 2);
    shader:set_int("material.emission_map", 3);
end

function update()
end