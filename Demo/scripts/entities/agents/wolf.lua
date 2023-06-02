function init()
    local shaders = {};
    local entities = {};
    models = {};

    for i = 1, 2 do
        if (i == 1) then
            entities[i] = entity;
        else
            entities[i] = scene:create_entity("wolf_instance ".. i, "");
        end

        entities[i]:get_transform().pos = Vector3.new(1.0 * i, 0.0, -5.0);

        local pos = entities[i]:get_transform().pos;
        
        local terr_scale = scene.globals.terrain.transform.scale;
        pos.y = (scene.globals.terrain.terr:get_height(pos.x / terr_scale.x, pos.z / terr_scale.z) * terr_scale.y) + 0.35;

	    models[i] = entities[i]:add_model("assets/models/wolfleader.fbx", 1.0, true, true);
	    entities[i]:set_shader("coreAssets/shaders/animation.prog");

	    shaders[i] = models[i]:get_shader();
	    shaders[i]:set_vec3v("light.direction", Vector3.new(math.rad(0.0), math.rad(0.0), math.rad(-90.0)));

        shaders[i]:set_float("material.shininess", 32.0);

        shaders[i]:set_int("material.diffuse_map", 0);
        shaders[i]:set_int("material.specular_map", 1);
        shaders[i]:set_int("material.normal_map", 2);
        shaders[i]:set_int("material.emission_map", 3);

        state.fsm = entities[i]:add_fsm();
    end
end

function update()
end