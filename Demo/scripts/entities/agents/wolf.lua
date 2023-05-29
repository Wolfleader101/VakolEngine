function init()
    local shaders = {};

    entity:get_transform().pos = Vector3.new(0.0, 0.75, -5.0);

	state.model = entity:add_model("assets/models/wolfleader.fbx", 1.0, true, true)
	entity:set_shader("coreAssets/shaders/animation.prog")

	shaders[1] = state.model:get_shader();
	shaders[1]:set_vec3v("light.direction", Vector3.new(math.rad(0.0), math.rad(0.0), math.rad(-90.0)));

    shaders[1]:set_float("material.shininess", 32.0);
    shaders[1]:set_vec3v("tint", Vector3.new(0.2, 0.65, 0.9));

    shaders[1]:set_int("material.diffuse_map", 0);
    shaders[1]:set_int("material.specular_map", 1);
    shaders[1]:set_int("material.normal_map", 2);
    shaders[1]:set_int("material.emission_map", 3);

    state.fsm = entity:add_fsm();

    IDLE = false;
end

function update()
    --state.model:update_animation(Time.delta_time);
end