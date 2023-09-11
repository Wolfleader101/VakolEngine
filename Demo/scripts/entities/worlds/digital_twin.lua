function init()
	GLOBAL_SCALE = 0.02;

	--OBJECT POSITIONS
	local benchPositions = 
	{
		Vector3.new(-35.0, 1.02, 114.7), Vector3.new(-27.6, 1.02, 114.54), Vector3.new(-21.1, 1.02, 114.58),
		Vector3.new(-21.5, 1.02, 123.6), Vector3.new(-27.9, 1.02, 123.7), Vector3.new(-21.5, 1.02, 132.58), 
		Vector3.new(-28.6, 1.02, 131.8)
	};

	local tablePositions = 
	{
		Vector3.new(-17.9, 1.04, 44.2), Vector3.new(-21.2, 1.04, 48.7), Vector3.new(-21.3, 1.04, 54.1),
		Vector3.new(-20.9, 1.04, 60.8)
	};

	local chairRotations = 
	{
		Vector3.new(0.0, -102.0, 0.0), Vector3.new(0.0, 87.0, 0.0), Vector3.new(0.0, 92.6, 0.0),
		Vector3.new(0.0, 254.2, 0.0), Vector3.new(0.0, 280.4, 0.0), Vector3.new(0.0, 271.4, 0.0),
		Vector3.new(0.0, 433.4, 0.0), Vector3.new(0.0, 89.0, 0.0), Vector3.new(0.0, 94.0, 0.0),
		Vector3.new(0.0, 90.6, 0.0), Vector3.new(0.0, 270.7, 0.0), Vector3.new(0.0, 262.8, 0.0),
		Vector3.new(0.0, 273.5, 0.0), Vector3.new(0.0, -82.1, 0.0), Vector3.new(0.0, 73.1, 0.0),
		Vector3.new(0.0, 92.2, 0.0), Vector3.new(0.0, 175.6, 0.0), Vector3.new(0.0, 182.1, 0.0),
		Vector3.new(0.0, 180.2, 0.0), Vector3.new(0.0, 174.3, 0.0)
	};

	local chairPositions = 
	{
		Vector3.new(-17.3, 1.16, 45.6), Vector3.new(-17.3, 1.16, 42.6), Vector3.new(-18.6, 1.16, 43.0),
		Vector3.new(-19.0, 1.16, 46.3), Vector3.new(-20.8, 1.16, 49.7), Vector3.new(-22.1, 1.16, 50.0),
		Vector3.new(-21.9, 1.16, 47.5), Vector3.new(-20.6, 1.16, 47.9), Vector3.new(-20.4, 1.16, 53.3),
		Vector3.new(-22.0, 1.16, 53.0), Vector3.new(-20.8, 1.16, 55.2), Vector3.new(-22.1, 1.16, 55.4),
		Vector3.new(-20.2, 1.16, 61.7), Vector3.new(-21.6, 1.16, 61.9), Vector3.new(-20.1, 1.16, 59.8),
		Vector3.new(-21.7, 1.16, 59.6), Vector3.new(-17.2, 1.16, 56.5), Vector3.new(-17.5, 1.16, 58.5),
		Vector3.new(-17.3, 1.16, 60.2), Vector3.new(-17.4, 1.16, 61.9)
	}

	local rubbishBinPositions = 
	{
		Vector3.new(33.0, 1.21, 39.0), Vector3.new(-16.3, 1.21, 84.0)
	}

	local recyclingBinPositions = 
	{
		Vector3.new(31.4, 1.21, 39.0), Vector3.new(-16.3, 1.21, 85.6)
	}

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

	colliders[14]:get_transform().pos = Vector3.new(-38, 1.25, -18.5);
	colliders[14]:add_sphere_collider(2.0);

	colliders[15]:get_transform().pos = Vector3.new(-38.0, 1.25, -6.2);
	colliders[15]:add_sphere_collider(2.0);

	colliders[16]:get_transform().pos = Vector3.new(-38.0, 1.25, 6.2);
	colliders[16]:add_sphere_collider(2.0);

	colliders[17]:get_transform().pos = Vector3.new(-38.0, 1.25, 18.5);
	colliders[17]:add_sphere_collider(2.0);

	colliders[18]:get_transform().pos = Vector3.new(-38.0, -4.0, 30.85);
	colliders[18]:add_sphere_collider(2.0);

	colliders[19]:get_transform().pos = Vector3.new(-38.0, -9.0, 43.25);
	colliders[19]:add_sphere_collider(2.0);

	colliders[20]:get_transform().pos = Vector3.new(-38.0, -9.5, 55.55);
	colliders[20]:add_sphere_collider(2.0);

	colliders[21]:get_transform().pos = Vector3.new(-38.0, -9.5, 67.9);
	colliders[21]:add_sphere_collider(2.0);
end

function create_floor_colliders()
	local colliders = {};

	for i = 1, 2 do
		colliders[i] = scene:create_entity("Floor Collider " .. i, "");

		local rigid = colliders[i]:add_rigid();
		rigid.type = BodyType.Static;
	end

	colliders[1]:get_transform().pos = Vector3.new(4.0, -0.05, 30.0);
	colliders[1]:add_box_collider(Vector3.new(32.0, 0.1, 115.0));

	colliders[2]:get_transform().pos = Vector3.new(-45.0, -0.05, -31.7);
	colliders[2]:add_box_collider(Vector3.new(17.0, 0.1, 53.4));
end

function create_building_colliders()
	local colliders = {};

	for i = 1, 4 do
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

	colliders[4]:get_transform().pos = Vector3.new(-25.5, 5.1, 141.4);
	colliders[4]:add_box_collider(Vector3.new(10.0, 5.0, 0.1));
end

function create_barrier_colliders()
	local colliders = {};

	for i = 1, 5 do
		colliders[i] = scene:create_entity("Left Barrier Collider " .. i, "");

		local rigid = colliders[i]:add_rigid();
		rigid.type = BodyType.Static;

		colliders[i]:get_transform().pos = Vector3.new(37.9, 1.5, 0.0);
		colliders[i]:add_box_collider(Vector3.new(0.3, 1.3, 5.1));
	end

	colliders[1]:get_transform().pos.z = -12.5;
	colliders[2]:get_transform().pos.z = 0.0;
	colliders[3]:get_transform().pos.z = 12.5;
	colliders[4]:get_transform().pos.z = 24.5;
	colliders[5]:get_transform().pos.z = 36.9;

	local colliders = {};

	for i = 1, 2 do
		colliders[i] = scene:create_entity("Right Barrier Collider " .. i, "");

		local rigid = colliders[i]:add_rigid();
		rigid.type = BodyType.Static;
	end

	colliders[1]:get_transform().pos = Vector3.new(-28.2, 1.5, 67.4);
	colliders[1]:add_box_collider(Vector3.new(0.3, 1.3, 43.7));

	colliders[2]:get_transform().pos = Vector3.new(-30.5, 1.5, 111.3);
	colliders[2]:add_box_collider(Vector3.new(1.950, 1.3, 0.30));
end

function update()
end