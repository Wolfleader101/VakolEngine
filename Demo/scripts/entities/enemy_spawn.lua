function init()
	state.model = load_model("assets/models/enemy.glb", 0.003, true, true);
	state.model:set_shader("coreAssets/shaders/animation.prog");

    entity:add_shader_storage_buffer_data(state.model:get_num_anim_transforms() * 64, 3, state.model:get_anim_transforms());

    state.model:set_animation_state(0);

    local shader = state.model:get_shader();

    shader:set_vec3v("light.direction", Vector3.new(math.rad(0.0), math.rad(0.0), math.rad(90.0)));

    shader:set_float("material.shininess", 16.0);
    shader:set_vec3v("tint", Vector3.new(0.2, 0.65, 0.9));

    shader:set_int("material.diffuse_map", 0);
    shader:set_int("material.specular_map", 1);
    shader:set_int("material.normal_map", 2);
    shader:set_int("material.emission_map", 3);
end


function update()
    state.model:update_animation(Time.delta_time);
end