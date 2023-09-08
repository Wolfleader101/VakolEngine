function init()
	GLOBAL_SCALE = 0.02;

	local floor = scene:create_entity("Floor", "");

	local roof = scene:create_entity("Roof", "");
	local roof_supports = scene:create_entity("Roof Supports", "");
	
	local building = scene:create_entity("Building", "");
	
	local barriers = {};
	
	local left_pillars = scene:create_entity("Left Pillars", "");
	local right_pillars = scene:create_entity("Right Pillars", "");

	local balcony = scene:create_entity("Balcony", "");

	local table_top = scene:create_entity("Table Top", "");
	
	local windowsill = scene:create_entity("Windowsill", "");

	barriers[1] = scene:create_entity("Glass Barrier Left", "");
	barriers[2] = scene:create_entity("Glass Barrier Right", "");

	floor:add_model("assets/models/digital_twin/GROUND.fbx", GLOBAL_SCALE);
	floor:get_transform().pos = Vector3.new(0.0, 0.0, 0.0);
	floor:get_transform().scale = Vector3.new(26.177, 1.0, 32.2);

	roof:add_model("assets/models/digital_twin/ROOF.fbx", GLOBAL_SCALE);
	roof_supports:add_model("assets/models/digital_twin/ROOF-SUPPORTS.fbx", GLOBAL_SCALE);

	building:add_model("assets/models/digital_twin/BUILDING.fbx", GLOBAL_SCALE);

	barriers[1]:add_model("assets/models/digital_twin/GLASS-BARRIERS-LEFT.fbx", GLOBAL_SCALE);
	barriers[2]:add_model("assets/models/digital_twin/GLASS-BARRIERS-RIGHT.fbx", GLOBAL_SCALE);

	left_pillars:add_model("assets/models/digital_twin/WOODEN-PILLARS-LEFT.fbx", GLOBAL_SCALE);
	right_pillars:add_model("assets/models/digital_twin/WOODEN-PILLARS-RIGHT.fbx", GLOBAL_SCALE);

	balcony:add_model("assets/models/digital_twin/BALCONY.fbx", GLOBAL_SCALE);

	table_top:add_model("assets/models/digital_twin/TABLE-TOP.fbx", GLOBAL_SCALE);

	windowsill:add_model("assets/models/digital_twin/WINDOWSILL.fbx", GLOBAL_SCALE);

	building:get_transform().pos.z = 1.80;

	roof:get_transform().pos.y = 31.0;
	roof_supports:get_transform().pos.y = -1.0;

	barriers[2]:get_transform().pos = Vector3.new(0.1, 0.0, 2.0);

	balcony:get_transform().pos.z = 2.25;

	windowsill:get_transform().pos.z = 1.85;

	create_pillar_colliders();
	create_floor_colliders();
	create_building_colliders();
end

function create_pillar_colliders()
	local colliders = {};

	for i = 1, 5 do
		colliders[i] = scene:create_entity("Left Pillar Collider " .. i, "");

		local rigid = colliders[i]:add_rigid();
		rigid.type = BodyType.Static;

		colliders[i]:add_box_collider(Vector3.new(1.0, 9.0, 0.7));

		colliders[i]:get_transform().pos = Vector3.new(37.7, 9.0, 0.0);
	end

	colliders[1]:get_transform().pos.z = -18.50;
	colliders[2]:get_transform().pos.z = -6.2;
	colliders[3]:get_transform().pos.z = 6.2;
	colliders[4]:get_transform().pos.z = 18.5;
	colliders[5]:get_transform().pos.z = 30.85;

	for i = 6, 10 do
		colliders[i] = scene:create_entity("Right Pillar Collider " .. i, "");

		local rigid = colliders[i]:add_rigid();
		rigid.type = BodyType.Static;

		colliders[i]:add_box_collider(Vector3.new(1.0, 9.0, 0.7));

		colliders[i]:get_transform().pos = Vector3.new(-37.7, 9.0, 0.0);
	end

	colliders[6]:get_transform().pos.z = -18.50;
	colliders[7]:get_transform().pos.z = -6.2;
	colliders[8]:get_transform().pos.z = 6.2;
	colliders[9]:get_transform().pos.z = 18.5;
	colliders[10]:get_transform().pos.z = 30.85;
end

function create_floor_colliders()
	local colliders = {};

	for i = 1, 1 do
		colliders[i] = scene:create_entity("Floor Collider " .. i, "");

		local rigid = colliders[i]:add_rigid();
		rigid.type = BodyType.Static;
	end

	colliders[1]:get_transform().pos = Vector3.new(4.0, -0.05, 30.0);
	colliders[1]:add_box_collider(Vector3.new(32.0, 0.1, 115.0));
end

function create_building_colliders()
	local colliders = {};

	for i = 1, 3 do
		colliders[i] = scene:create_entity("Building Collider " .. i, "");

		local rigid = colliders[i]:add_rigid();
		rigid.type = BodyType.Static;
	end

	colliders[1]:get_transform().pos = Vector3.new(-5.6, 14.1, 90.9);
	colliders[1]:add_box_collider(Vector3.new(9.75, 14.0, 50.0));

	colliders[2]:get_transform().pos = Vector3.new(14.8, 14.05, 43.3);
	colliders[2]:add_box_collider(Vector3.new(10.4, 14.0, 5.0));
	
	colliders[3]:get_transform().pos = Vector3.new(32.3, 14.05, 43.1);
	colliders[3]:add_box_collider(Vector3.new(6.8, 14.0, 1.0));
end

function update()
end