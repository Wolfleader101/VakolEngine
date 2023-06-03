function init()
	local WATER_LEVEL = 10.0;

	local n_trees = 100;
	local n_rocks = 300;

	local m_trees = vector_mat4(n_trees);
	local m_rocks = vector_mat4(n_rocks);

	local entities = {};
	local models = {};
	local shaders = {};

	entities[1] = entity;
	models[1] = entities[1]:add_model("coreAssets/models/Imported/OpenGameArt/Yughues/pine/snow_pine_tree.obj", 0.1, false);
	entities[1]:set_shader("coreAssets/shaders/instance.prog");

	shaders[1] = models[1]:get_shader();

	for i = 1, n_trees do
		local mdl_m = Matrix4x4.new(1.0);

		local scl_v = Vector3.new(1.0);
		local pos_v = Vector3.new(0.0);

		pos_v = Vector3.new(math.random(-72.0, 72.0), 0.0, math.random(-72.0, 72.0));

        local terr_scale = scene.globals.terrain.transform.scale;
        pos_v.y = (scene.globals.terrain.terr:get_height(pos_v.x / terr_scale.x, pos_v.z / terr_scale.z) * terr_scale.y) + 0.015;

		mdl_m = translate(mdl_m, pos_v);

        mdl_m = rotate(mdl_m, math.rad(0.0), Vector3.new(1.0, 0.0, 0.0));
        mdl_m = rotate(mdl_m, math.rad(0.0), Vector3.new(0.0, 1.0, 0.0));
        mdl_m = rotate(mdl_m, math.rad(0.0), Vector3.new(0.0, 0.0, 1.0));

		mdl_m = scale(mdl_m, scl_v);

		m_trees[i] = mdl_m;
	end

	--instantiate_model(models[1], m_trees, n_trees);

	entities[2] = scene:create_entity("rock", "");
	models[2] = entities[2]:add_model("coreAssets/models/Imported/OpenGameArt/mastahcez/stone.fbx", 75.0, true);
	entities[2]:set_shader("coreAssets/shaders/instance.prog");

	shaders[2] = models[2]:get_shader();

	for i = 1, n_rocks do
		local mdl_m = Matrix4x4.new(1.0);

		local scl_v = Vector3.new(math.random(1, 75) / 100);
		local pos_v = Vector3.new(0.0);

		pos_v = Vector3.new(math.random(-64.0, 64.0), 0.0, math.random(-64.0, 64.0));

        local terr_scale = scene.globals.terrain.transform.scale;
        pos_v.y = (scene.globals.terrain.terr:get_height(pos_v.x / terr_scale.x, pos_v.z / terr_scale.z) * terr_scale.y) + 0.05;

		mdl_m = translate(mdl_m, pos_v);

        mdl_m = rotate(mdl_m, math.rad(0.0), Vector3.new(1.0, 0.0, 0.0));
        mdl_m = rotate(mdl_m, math.rad(0.0), Vector3.new(0.0, 1.0, 0.0));
        mdl_m = rotate(mdl_m, math.rad(0.0), Vector3.new(0.0, 0.0, 1.0));

		mdl_m = scale(mdl_m, scl_v);

		m_rocks[i] = mdl_m;
	end

	instantiate_model(models[2], m_rocks, n_rocks);
end

function update()
end