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

	colliders[14]:get_transform().pos = Vector3.new(-38, 2.05, -18.5);
	colliders[14]:add_sphere_collider(2.0);

	colliders[15]:get_transform().pos = Vector3.new(-38.0, 2.05, -6.2);
	colliders[15]:add_sphere_collider(2.0);

	colliders[16]:get_transform().pos = Vector3.new(-38.0, 2.05, 6.2);
	colliders[16]:add_sphere_collider(2.0);

	colliders[17]:get_transform().pos = Vector3.new(-38.0, 2.05, 18.5);
	colliders[17]:add_sphere_collider(2.0);

	colliders[18]:get_transform().pos = Vector3.new(-38.0, -4.0, 30.85);
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

	for i = 1, 4 do
		colliders[i] = scene:create_entity("Floor Collider " .. i, "");

		local rigid = colliders[i]:add_rigid();
		rigid.type = BodyType.Static;
	end

	colliders[1]:get_transform().pos = Vector3.new(4.0, 0.01, 30.0);
	colliders[1]:add_box_collider(Vector3.new(32.0, 0.01, 115.0));

	colliders[2]:get_transform().pos = Vector3.new(-45.0, 0.01, -31.7);
	colliders[2]:add_box_collider(Vector3.new(17.0, 0.01, 53.4));

	colliders[3]:get_transform().pos = Vector3.new(-34.01, 0.01, 124.3);
	colliders[3]:add_box_collider(Vector3.new(6.0, 0.01, 15.2));

	colliders[4]:get_transform().pos = Vector3.new(-44.8, -10.57, 74.5);
	colliders[4]:add_box_collider(Vector3.new(15.8, 0.01, 34.1));
end

function create_building_colliders()
	local colliders = {};

	for i = 1, 4 do
		colliders[i] = scene:create_entity("Building Collider " .. i, "");

		local rigid = colliders[i]:add_rigid();
		rigid.type = BodyType.Static;
	end

	colliders[1]:get_transform().pos = Vector3.new(-5.6, 14.1, 89.1);
	colliders[1]:add_box_collider(Vector3.new(9.75, 14.0, 50.0));

	colliders[2]:get_transform().pos = Vector3.new(14.8, 14.05, 41.6);
	colliders[2]:add_box_collider(Vector3.new(10.4, 14.0, 5.0));
	
	colliders[3]:get_transform().pos = Vector3.new(32.3, 14.05, 41.3);
	colliders[3]:add_box_collider(Vector3.new(6.8, 14.0, 1.0));

	colliders[4]:get_transform().pos = Vector3.new(-25.5, 5.1, 139.5);
	colliders[4]:add_box_collider(Vector3.new(10.0, 5.0, 0.1));
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

	local colliders = {};

	for i = 1, 2 do
		colliders[i] = scene:create_entity("Right Barrier Collider " .. i, "");

		local rigid = colliders[i]:add_rigid();
		rigid.type = BodyType.Static;
	end

	colliders[1]:get_transform().pos = Vector3.new(-28.2, 1.5, 65.7);
	colliders[1]:add_box_collider(Vector3.new(0.3, 1.3, 44.0));

	colliders[2]:get_transform().pos = Vector3.new(-30.5, 1.5, 109.3);
	colliders[2]:add_box_collider(Vector3.new(1.950, 1.3, 0.30));
end

function update()
end