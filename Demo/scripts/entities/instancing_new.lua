function init()
	local MAX_ATTEMPTS = 50;

	local n_trees = 2500;
	local n_rocks = 2500;

	local attempts = 0;

	local m_trees = vector_mat4(n_trees);
	local m_rocks = vector_mat4(n_rocks);

	local entities = {};
	local models = {};
	local shaders = {};

	entities[1] = scene:create_entity("trees", "");
	models[1] = entities[1]:add_model("assets/models/Imported/OpenGameArt/Yughues/pine/snow_pine_tree.obj", 0.2, false, false);
	entities[1]:set_shader("coreAssets/shaders/instance.prog");

	shaders[1] = models[1]:get_shader();
    
    local terr_scale = scene.globals.terrain.transform.scale;

	for i = 1, n_trees do
		local mdl_m = Matrix4x4.new(1.0);

        local scl_v = Vector3.new(0.2);
		local pos_v = Vector3.new(0.0);

		attempts = 0;

		while(pos_v.y < scene.globals.waterlevel) do
			if attempts > MAX_ATTEMPTS then
				break;
			end

			pos_v = Vector3.new(math.random(-(math.floor(scene.globals.terrain.terr:get_size() / 2)), math.floor(scene.globals.terrain.terr:get_size() / 2)), 
				0.0, math.random(-(math.floor(scene.globals.terrain.terr:get_size() / 2)), math.floor(scene.globals.terrain.terr:get_size() / 2)));

			pos_v.y = (scene.globals.terrain.terr:get_height(pos_v.x / terr_scale.x, pos_v.z / terr_scale.z) * terr_scale.y) + 0.015;

			attempts = attempts + 1
		end

        local tree_ent = scene:create_entity("tree  " .. i, "");

        tree_ent:get_transform().pos = pos_v;
        -- tree_ent:get_transform().scale = scl_v;
        -- !fuck this hes cursed our code, setting the tree ent scale fucking sets the terrains scale?!?!?!
		--TODO FUCKING FUCK THIS


		local rb = tree_ent:add_rigid();
		rb.BodyType = BodyType.Static;

		local collider = tree_ent:add_collider();

		collider.Shape = Shape.Box;
		collider.bounds.extents.x = scl_v.x;
		collider.bounds.extents.y = scl_v.y;
		collider.bounds.extents.z = scl_v.z;
		tree_ent:physics_init(scene); 
		

		mdl_m = translate(mdl_m, pos_v);

		mdl_m = rotate(mdl_m, math.rad(0.0), Vector3.new(1.0, 0.0, 0.0));
		mdl_m = rotate(mdl_m, math.rad(0.0), Vector3.new(0.0, 1.0, 0.0));
		mdl_m = rotate(mdl_m, math.rad(0.0), Vector3.new(0.0, 0.0, 1.0));

		mdl_m = scale(mdl_m, scl_v);

		m_trees[i] = mdl_m;
	end

	instantiate_model(models[1], m_trees, n_trees);

	entities[2] = scene:create_entity("rocks", "");
	models[2] = entities[2]:add_model("assets/models/Imported/OpenGameArt/mastahcez/stone.fbx", 75.0, false, true);
	entities[2]:set_shader("coreAssets/shaders/instance.prog");

	shaders[2] = models[2]:get_shader();

	for i = 1, n_rocks do
		local mdl_m = Matrix4x4.new(1.0);

		local scl_v = Vector3.new(math.random(1, 75) / 100);
		local pos_v = Vector3.new(0.0);

		attempts = 0;

        while (pos_v.y < scene.globals.waterlevel)
        do
            if attempts > MAX_ATTEMPTS then
                break;
            end

            pos_v = Vector3.new(
                math.random(-(math.floor(scene.globals.terrain.terr:get_size() / 2)),
                    math.floor(scene.globals.terrain.terr:get_size() / 2)),
                0.0,
                math.random(-(math.floor(scene.globals.terrain.terr:get_size() / 2)),
                    math.floor(scene.globals.terrain.terr:get_size() / 2)));

            pos_v.y = (scene.globals.terrain.terr:get_height(pos_v.x / terr_scale.x, pos_v.z / terr_scale.z) * terr_scale.y) +
            0.015;

            attempts = attempts + 1
        end
		
		local rock_ent = scene:create_entity("rock" .. i, "");

		rock_ent:get_transform().pos = pos_v;
		rock_ent:get_transform().scale = scl_v; 

		local rb = rock_ent:add_rigid();
		rb.BodyType = BodyType.Static;

		local collider = rock_ent:add_collider();

		collider.Shape = Shape.Box;
		collider.bounds.extents.x = scl_v.x;
		collider.bounds.extents.y = scl_v.y;
		collider.bounds.extents.z = scl_v.z;
		rock_ent:physics_init(scene); 
		

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