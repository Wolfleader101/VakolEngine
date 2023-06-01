function init()
    entity:get_transform().pos = Vector3.new(4.0, 0.60, -2.0);

	state.model = entity:add_model("assets/models/rabbit.fbx", 0.005, true, true)
	entity:set_shader("coreAssets/shaders/animation.prog")

	local shader = state.model:get_shader();
	shader:set_vec3v("light.direction", Vector3.new(math.rad(0.0), math.rad(0.0), math.rad(-90.0)));

    shader:set_float("material.shininess", 32.0);

    shader:set_int("material.diffuse_map", 0);
    shader:set_int("material.specular_map", 1);
    shader:set_int("material.normal_map", 2);
    shader:set_int("material.emission_map", 3);

    state.fsm = entity:add_fsm();

    entity:set_animation_state(0);
end

function update()
end