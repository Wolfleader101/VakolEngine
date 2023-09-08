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
end

function update()
end