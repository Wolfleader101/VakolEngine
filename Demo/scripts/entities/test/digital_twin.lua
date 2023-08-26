function init()
	local roof = scene:create_entity("Roof", "");

	local roof_support = scene:create_entity("Roof Support", "");

	local building = scene:create_entity("Building", "");

	local floor = scene:create_entity("Floor", "");
	
	local alcove_1 = scene:create_entity("Alcove 1", "");
	local alcove_2 = scene:create_entity("Alcove 2", "");

	local glass_panel1 = scene:create_entity("Glass Panel 1", "");
	local glass_panel2 = scene:create_entity("Glass Panel 2", "");

	local wood_pillars = {};
	local stone_pillars = {};

	local barriers = {};

	local balcony = scene:create_entity("Balcony", "");

	for i = 1, 6 do
		wood_pillars[i] = scene:create_entity("Wood Pillar " .. i, "");
	end

	for i = 1, 6 do
		stone_pillars[i] = scene:create_entity("Stone Pillar " .. i, "");
	end

	for i = 1, 2 do
		barriers[i] = scene:create_entity("Glass Barrier " .. i, "");
	end

	roof:add_model("assets/models/new_building/roof.fbx", 0.005);
	roof_support:add_model("assets/models/new_building/roof_supports.fbx", 0.5);
	
	building:add_model("assets/models/new_building/building.fbx", 0.005);
	
	floor:add_model("assets/models/new_building/floor.fbx", 0.005);
	
	alcove_1:add_model("assets/models/new_building/alcove1.fbx", 0.005);
	alcove_2:add_model("assets/models/new_building/alcove2.fbx", 0.005);

	glass_panel1:add_model("assets/models/new_building/glass_panel1.fbx", 0.005);
	glass_panel2:add_model("assets/models/new_building/glass_panel2.fbx", 0.005);

	for i = 1, 6 do
		wood_pillars[i]:add_model("assets/models/new_building/wood_pillar.fbx", 0.30);
	end

	for i = 1, 6 do
		stone_pillars[i]:add_model("assets/models/new_building/stone_pillar.fbx", 0.50);
	end

	for i = 1, 2 do
		barriers[i]:add_model("assets/models/new_building/glass_barrier.fbx", 0.010);
	end

	balcony:add_model("assets/models/new_building/balcony.fbx", 0.2);

	roof:get_transform().pos = Vector3.new(0.0, 7.5, 0.0);
	roof:get_transform().scale = Vector3.new(1.0, 1.0, 1.0);

	roof_support:get_transform().pos = roof:get_transform().pos - Vector3.new(0.0, 0.515, -13.75);
	
	building:get_transform().pos = Vector3.new(2.0, -1.0, 9.295);
	building:get_transform().rot = Vector3.new(-90.0, 0.0, 0.0);
	building:get_transform().scale = Vector3.new(28.415, 1.0, 1.0);

	floor:get_transform().pos = Vector3.new(-1.0, -1.251, 1.0);
	floor:get_transform().rot = Vector3.new(0.0, 0.0, 0.0);
	floor:get_transform().scale = Vector3.new(30.0, 1.0, 30.0);

	alcove_1:get_transform().pos = Vector3.new(-10.0, -1.0, 28.70);
	alcove_1:get_transform().scale = Vector3.new(0.90, 1.0, 0.890);

	alcove_2:get_transform().pos = Vector3.new(-12.65, -1.25, 27.870);
	alcove_2:get_transform().scale = Vector3.new(0.90, 1.0, 0.890);

	glass_panel1:get_transform().pos = Vector3.new(-10.675, -1.0, 9.35);
	glass_panel1:get_transform().rot = Vector3.new(-90.0, 0.0, 0.0);

	glass_panel2:get_transform().pos = Vector3.new(-10.675, -1.0, 9.30);
	glass_panel2:get_transform().rot = Vector3.new(-90.0, 0.0, 0.0);

	for i = 1, 6 do
		wood_pillars[i]:get_transform().pos = Vector3.new(10.0, 1.75, 0.0);
		wood_pillars[i]:get_transform().rot = Vector3.new(0.0, 90.0, 0.0);
		wood_pillars[i]:get_transform().scale = Vector3.new(1.0, 1.0, 1.0);
	end

	wood_pillars[1]:get_transform().pos.z = -1.95;
	wood_pillars[2]:get_transform().pos.z = 3.25;
	wood_pillars[3]:get_transform().pos.z = 8.50;
	wood_pillars[4]:get_transform().pos.z = 13.75;
	wood_pillars[5]:get_transform().pos.z = 19.0;
	wood_pillars[6]:get_transform().pos.z = 24.25;

	for i = 1, 6 do
		stone_pillars[i]:get_transform().pos = Vector3.new(-10.0, -0.25, 0.0);
		stone_pillars[i]:get_transform().rot = Vector3.new(0.0, -90.0, 0.0);
		stone_pillars[i]:get_transform().scale = Vector3.new(1.0, 1.0, 1.0);
	end

	stone_pillars[1]:get_transform().pos = Vector3.new(-10.0, -0.25, -1.95);
	stone_pillars[1]:get_transform().scale.y = 1.2;

	stone_pillars[2]:get_transform().pos = Vector3.new(-10.0, -0.25, 3.25);
	stone_pillars[2]:get_transform().scale.y = 1.2;

	stone_pillars[3]:get_transform().pos = Vector3.new(-10.0, -1.25, 8.50);
	stone_pillars[3]:get_transform().scale.y = 1.6;

	stone_pillars[4]:get_transform().pos = Vector3.new(-10.0, -2.20, 13.75);
	stone_pillars[4]:get_transform().scale.y = 1.8;

	stone_pillars[5]:get_transform().pos = Vector3.new(-10.0, -2.20, 19.0);
	stone_pillars[5]:get_transform().scale.y = 1.8;

	stone_pillars[6]:get_transform().pos = Vector3.new(-10.0, -2.20, 24.25);
	stone_pillars[6]:get_transform().scale.y = 1.8;

	barriers[1]:get_transform().pos = Vector3.new(10.7, -1.230, 14.0);
	barriers[1]:get_transform().rot = Vector3.new(0.0, 90.0, 0.0);

	barriers[2]:get_transform().pos = Vector3.new(-9.9, -1.230, 9.5);
	barriers[2]:get_transform().rot = Vector3.new(0.0, -90.0, 0.0);
	barriers[2]:get_transform().scale.z = 1.09;

	balcony:get_transform().pos = Vector3.new(-7.20, 2.15, 13.40);
	balcony:get_transform().scale = Vector3.new(2.10, 1.0, 2.6);
end

function update()
end