function init()
	GLOBAL_SCALE = 0.02;

	--ENTITY CREATION
	--Digital Twin Geometry Entities
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

	barriers[3] = scene:create_entity("Glass Barrier 1", "");
	barriers[4] = scene:create_entity("Glass Barrier 2", "");
	barriers[5] = scene:create_entity("Glass Barrier 3", "");
	barriers[6] = scene:create_entity("Glass Barrier 4", "");
	barriers[7] = scene:create_entity("Glass Barrier 5", "");

	--Add Props to Digital Twin
	local props = scene:create_entity("Props", "entities/worlds/digital_twin_props.lua");

	--MODEL FILE ACQUISITION
	--Digital Twin Geometry Models
	floor:add_model("assets/models/digital_twin/GROUND.fbx", GLOBAL_SCALE);

	roof:add_model("assets/models/digital_twin/ROOF.fbx", GLOBAL_SCALE);
	roof_supports:add_model("assets/models/digital_twin/ROOF-SUPPORTS.fbx", GLOBAL_SCALE);

	building:add_model("assets/models/digital_twin/BUILDING.fbx", GLOBAL_SCALE);

	barriers[1]:add_model("assets/models/digital_twin/GLASS-BARRIERS-LEFT.fbx", GLOBAL_SCALE);
	barriers[2]:add_model("assets/models/digital_twin/GLASS-BARRIERS-RIGHT.fbx", GLOBAL_SCALE);
	barriers[3]:add_model("assets/models/digital_twin/GLASS-BARRIERS-LEFT.fbx", GLOBAL_SCALE);
	local b_mdl_4 = barriers[4]:add_model("assets/models/digital_twin/GLASS-BARRIERS-LEFT.fbx", GLOBAL_SCALE);
	b_mdl_4:get_mesh(0).material:set_light_position(Vector3.new(180.0, 0.0, -180.0));
	barriers[5]:add_model("assets/models/digital_twin/GLASS-BARRIERS-LEFT.fbx", GLOBAL_SCALE);
	barriers[6]:add_model("assets/models/digital_twin/GLASS-BARRIERS-LEFT.fbx", GLOBAL_SCALE);
	barriers[7]:add_model("assets/models/digital_twin/GLASS-BARRIERS-LEFT.fbx", GLOBAL_SCALE);

	left_pillars:add_model("assets/models/digital_twin/WOODEN-PILLARS-LEFT.fbx", GLOBAL_SCALE);

	right_pillars:add_model("assets/models/digital_twin/WOODEN-PILLARS-RIGHT.fbx", GLOBAL_SCALE);

	balcony:add_model("assets/models/digital_twin/BALCONY.fbx", GLOBAL_SCALE);

	table_top:add_model("assets/models/digital_twin/TABLE-TOP.fbx", GLOBAL_SCALE);

	windowsill:add_model("assets/models/digital_twin/WINDOWSILL.fbx", GLOBAL_SCALE);

	--COLLIDER FUNCTIONS
	create_pillar_colliders();
	create_floor_colliders();
	create_building_colliders();
	create_barrier_colliders();
	create_stair_colliders();

	--ADDITIONAL MODIFICATIONS
	barriers[3]:get_transform().pos = Vector3.new(0.0, 0.0, -64.7);
	barriers[3]:get_transform().scale = Vector3.new(1.0, 1.0, 1.06);

	barriers[4]:get_transform().pos = Vector3.new(-32.0, 0.0, -46.4);
	barriers[4]:get_transform().rot = Vector3.new(0.0, 90.0, 0.0);
	barriers[4]:get_transform().scale = Vector3.new(1.0, 1.0, 1.61);

	barriers[5]:get_transform().pos = Vector3.new(-23.9, 0.0, -9.7);
	barriers[5]:get_transform().rot = Vector3.new(0.0, 180.0, 0.0);
	barriers[5]:get_transform().scale = Vector3.new(1.0, 1.0, 1.72);

	barriers[6]:get_transform().pos = Vector3.new(-23.9, -10.56, 88.0);
	barriers[6]:get_transform().rot = Vector3.new(0.0, 180.0, 0.0);
	barriers[6]:get_transform().scale = Vector3.new(1.0, 1.0, 1.1);

	barriers[7]:get_transform().pos = Vector3.new(-18.5, -10.56, 70.3);
	barriers[7]:get_transform().rot = Vector3.new(0.0, 270.0, 0.0);
	barriers[7]:get_transform().scale = Vector3.new(1.0, 1.0, 1.0);
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

	colliders[1]:get_transform().pos.z = -18.5;
	colliders[2]:get_transform().pos.z = -6.2;
	colliders[3]:get_transform().pos.z = 6.2;
	colliders[4]:get_transform().pos.z = 18.5;
	colliders[5]:get_transform().pos.z = 30.85;

	for i = 6, 21 do
		colliders[i] = scene:create_entity("Right Pillar Collider " .. i, "");

		local rigid = colliders[i]:add_rigid();
		rigid.type = BodyType.Static;
	end

	colliders[6]:get_transform().pos = Vector3.new(-38.0, 11.0, -18.5);
	colliders[6]:add_box_collider(Vector3.new(1.0, 7.0, 0.7));

	colliders[7]:get_transform().pos = Vector3.new(-38.0, 11.0, -6.2);
	colliders[7]:add_box_collider(Vector3.new(1.0, 7.0, 0.7));

	colliders[8]:get_transform().pos = Vector3.new(-38.0, 11.0, 6.2);
	colliders[8]:add_box_collider(Vector3.new(1.0, 7.0, 0.7));

	colliders[9]:get_transform().pos = Vector3.new(-38.0, 11.0, 18.5);
	colliders[9]:add_box_collider(Vector3.new(1.0, 7.0, 0.7));
	
	colliders[10]:get_transform().pos = Vector3.new(-38.0, 7.85, 30.85);
	colliders[10]:add_box_collider(Vector3.new(1.0, 9.25, 0.7));

	colliders[11]:get_transform().pos = Vector3.new(-38.0, 5.4, 43.25);
	colliders[11]:add_box_collider(Vector3.new(1.0, 12.0, 0.7));

	colliders[12]:get_transform().pos = Vector3.new(-38.0, 5.0, 55.55);
	colliders[12]:add_box_collider(Vector3.new(1.0, 12.0, 0.7));
	
	colliders[13]:get_transform().pos = Vector3.new(-38.0, 5.0, 67.9);
	colliders[13]:add_box_collider(Vector3.new(1.0, 12.0, 0.7));

	colliders[14]:get_transform().pos = Vector3.new(-38, 2.05, -18.5);
	colliders[14]:add_sphere_collider(2.0);

	colliders[15]:get_transform().pos = Vector3.new(-38.0, 2.05, -6.2);
	colliders[15]:add_sphere_collider(2.0);

	colliders[16]:get_transform().pos = Vector3.new(-38.0, 2.05, 6.2);
	colliders[16]:add_sphere_collider(2.0);

	colliders[17]:get_transform().pos = Vector3.new(-38.0, 2.05, 18.5);
	colliders[17]:add_sphere_collider(2.0);

	colliders[18]:get_transform().pos = Vector3.new(-38.0, -3.05, 30.85);
	colliders[18]:add_sphere_collider(2.0);

	colliders[19]:get_transform().pos = Vector3.new(-38.0, -8.5, 43.25);
	colliders[19]:add_sphere_collider(2.0);

	colliders[20]:get_transform().pos = Vector3.new(-38.0, -8.5, 55.55);
	colliders[20]:add_sphere_collider(2.0);

	colliders[21]:get_transform().pos = Vector3.new(-38.0, -8.5, 67.9);
	colliders[21]:add_sphere_collider(2.0);
end

function create_floor_colliders()
	local colliders = {};

	for i = 1, 5 do
		colliders[i] = scene:create_entity("Floor Collider " .. i, "");

		local rigid = colliders[i]:add_rigid();
		rigid.type = BodyType.Static;
	end

	colliders[1]:get_transform().pos = Vector3.new(4.6, 0.0, 30.0);
	colliders[1]:add_box_collider(Vector3.new(32.0, 0.001, 115.0));

	colliders[2]:get_transform().pos = Vector3.new(-45.0, 0.0, -31.7);
	colliders[2]:add_box_collider(Vector3.new(17.0, 0.001, 53.4));

	colliders[3]:get_transform().pos = Vector3.new(-34.01, 0.01, 124.3);
	colliders[3]:add_box_collider(Vector3.new(6.0, 0.001, 15.2));

	colliders[4]:get_transform().pos = Vector3.new(-45.6, -5.26, 31.0);
	colliders[4]:add_box_collider(Vector3.new(16.5, 0.001, 3.8));

	colliders[5]:get_transform().pos = Vector3.new(-45.3, -10.56, 74.5);
	colliders[5]:add_box_collider(Vector3.new(15.8, 0.001, 34.1));
end

function create_building_colliders()
	local colliders = {};

	for i = 1, 13 do
		colliders[i] = scene:create_entity("Building Collider " .. i, "");

		local rigid = colliders[i]:add_rigid();
		rigid.type = BodyType.Static;
	end

	colliders[1]:get_transform().pos = Vector3.new(-5.6, 14.1, 89.1);
	colliders[1]:add_box_collider(Vector3.new(9.75, 14.0, 50.0));

	colliders[2]:get_transform().pos = Vector3.new(14.8, 14.05, 38.6);
	colliders[2]:add_box_collider(Vector3.new(10.4, 14.0, 2.0));
	
	colliders[3]:get_transform().pos = Vector3.new(32.3, 14.05, 41.3);
	colliders[3]:add_box_collider(Vector3.new(6.8, 14.0, 1.0));
	
	colliders[4]:get_transform().pos = Vector3.new(-25.0, 5.1, 139.5);
	colliders[4]:add_box_collider(Vector3.new(10.0, 5.0, 0.1));

	colliders[5]:get_transform().pos = Vector3.new(-35.4, 4.2, 133.5);
	colliders[5]:add_box_collider(Vector3.new(0.1, 4.15, 6.0));

	colliders[6]:get_transform().pos = Vector3.new(-34.7, 4.125, 126.61);
	colliders[6]:add_box_collider(Vector3.new(0.5, 4.1, 1.2));

	colliders[7]:get_transform().pos = Vector3.new(-35.4, 4.125, 123.0);
	colliders[7]:add_box_collider(Vector3.new(0.1, 4.1, 2.4));

	colliders[8]:get_transform().pos = Vector3.new(-34.6, 3.1, 119.99);
	colliders[8]:add_box_collider(Vector3.new(2.4, 3.0, 0.6));

	colliders[9]:get_transform().pos = Vector3.new(-39.0, 1.1, 118.2);
	colliders[9]:get_transform().rot = Vector3.new(0.0, 41.2, 0.0);
	colliders[9]:add_box_collider(Vector3.new(0.3, 1.0, 1.4));

	colliders[10]:get_transform().pos = Vector3.new(-40.0, 1.2, 114.7);
	colliders[10]:add_box_collider(Vector3.new(0.3, 0.9, 2.2));

	colliders[11]:get_transform().pos = Vector3.new(-38.8, 1.3, 111.2);
	colliders[11]:get_transform().rot = Vector3.new(0.0, -39.3, 0.0);
	colliders[11]:add_box_collider(Vector3.new(0.3, 0.9, 1.5));

	colliders[12]:get_transform().pos = Vector3.new(-34.9, 1.4, 109.9);
	colliders[12]:add_box_collider(Vector3.new(2.6, 1.3, 0.25));

	colliders[13]:get_transform().pos = Vector3.new(-28.9, -5.2, 67.8);
	colliders[13]:add_box_collider(Vector3.new(0.1, 5.2, 40.8));
end

function create_barrier_colliders()
	local colliders = {};

	for i = 1, 5 do
		colliders[i] = scene:create_entity("Left Barrier Collider " .. i, "");

		local rigid = colliders[i]:add_rigid();
		rigid.type = BodyType.Static;

		colliders[i]:get_transform().pos = Vector3.new(37.9, 1.5, 0.0);
	end

	colliders[1]:get_transform().pos.z = -12.5;
	colliders[1]:add_box_collider(Vector3.new(0.3, 1.3, 5.1));

	colliders[2]:get_transform().pos.z = 0.0;
	colliders[2]:add_box_collider(Vector3.new(0.3, 1.3, 5.1));

	colliders[3]:get_transform().pos.z = 12.5;
	colliders[3]:add_box_collider(Vector3.new(0.3, 1.3, 5.1));

	colliders[4]:get_transform().pos.z = 24.5;
	colliders[4]:add_box_collider(Vector3.new(0.3, 1.3, 5.1));

	colliders[5]:get_transform().pos = Vector3.new(37.9, 1.5, 35.9);
	colliders[5]:add_box_collider(Vector3.new(0.3, 1.3, 4.3));

	local count = 1;

	for i = 6, 7 do
		colliders[i] = scene:create_entity("Right Barrier Collider " .. count, "");

		local rigid = colliders[i]:add_rigid();
		rigid.type = BodyType.Static;

		count = count + 1;
	end

	colliders[6]:get_transform().pos = Vector3.new(-28.2, 1.5, 65.7);
	colliders[6]:add_box_collider(Vector3.new(0.3, 1.3, 44.0));

	colliders[7]:get_transform().pos = Vector3.new(-30.5, 1.5, 109.3);
	colliders[7]:add_box_collider(Vector3.new(1.950, 1.3, 0.30));

	count = 1;

	for i = 8, 12 do
		colliders[i] = scene:create_entity("Barrier Collider " .. count, "");

		local rigid = colliders[i]:add_rigid();
		rigid.type = BodyType.Static;

		count = count + 1;
	end

	colliders[8]:get_transform().pos = Vector3.new(37.9, 1.4, -51.8);
	colliders[8]:add_box_collider(Vector3.new(0.3, 1.3, 32.5));

	colliders[9]:get_transform().pos = Vector3.new(-12.3, 1.4, -84.3);
	colliders[9]:add_box_collider(Vector3.new(49.5, 1.3, 0.3));

	colliders[10]:get_transform().pos = Vector3.new(-61.8, 1.4, -31.4);
	colliders[10]:add_box_collider(Vector3.new(0.3, 1.3, 52.5));

	colliders[11]:get_transform().pos = Vector3.new(-62.0, -9.2, 74.5);
	colliders[11]:add_box_collider(Vector3.new(0.3, 1.3, 34.0));

	colliders[12]:get_transform().pos = Vector3.new(-45.4, -9.2, 108.3);
	colliders[12]:add_box_collider(Vector3.new(16.1, 1.3, 0.3));
end

function create_stair_colliders()
	local colliders = {};

	for i = 1, 2 do
		colliders[i] = scene:create_entity("Stair Collider " .. i, "");

		local rigid = colliders[i]:add_rigid();
		rigid.type = BodyType.Static;
	end

	colliders[1]:get_transform().pos = Vector3.new(-45.5, -2.9, 24.7);
	colliders[1]:get_transform().rot = Vector3.new(-46.4, 0.0, 0.0);
	colliders[1]:add_box_collider(Vector3.new(16.5, 4.0, 0.1));

	colliders[2]:get_transform().pos = Vector3.new(-45.5, -8.0, 37.7);
	colliders[2]:get_transform().rot = Vector3.new(-46.4, 0.0, 0.0);
	colliders[2]:add_box_collider(Vector3.new(16.5, 3.8, 0.1));
end

function update()
end