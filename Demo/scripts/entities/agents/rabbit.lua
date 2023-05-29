function init()
    entity:get_transform().pos = Vector3.new(4.0, 0.60, -2.0);
    entity:get_transform().rot = Vector3.new(0.0, -45.0, 0.0);
    entity:get_transform().scale = Vector3.new(0.5, 0.75, 0.5);

	state.model = entity:add_model("assets/models/cheems.glb", 0.25, true, true)
	entity:set_shader("coreAssets/shaders/animation.prog")

	local shader = state.model:get_shader();
	shader:set_vec3v("light.direction", Vector3.new(math.rad(0.0), math.rad(0.0), math.rad(-90.0)));

    shader:set_float("material.shininess", 32.0);

    shader:set_int("material.diffuse_map", 0);
    shader:set_int("material.specular_map", 1);
    shader:set_int("material.normal_map", 2);
    shader:set_int("material.emission_map", 3);

    state.fsm = entity:add_fsm();

    state.model:set_animation_state(0);
end

function update()
    state.model:update_animation(Time.delta_time);
end