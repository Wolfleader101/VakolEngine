function init()
	local roof = scene:create_entity("Roof", "");
	local roof_support = scene:create_entity("Roof Support", "");
	local building = scene:create_entity("Building", "");
	local floor = scene:create_entity("Floor", "");
	
	local alcove_1 = scene:create_entity("Alcove 1", "");
	local alcove_2 = scene:create_entity("Alcove 2", "");

	local glass_panel1 = scene:create_entity("Glass Panel 1", "");
	local glass_panel2 = scene:create_entity("Glass Panel 2", "");

	roof:add_model("assets/models/new_building/roof.fbx", 0.005);
	roof_support:add_model("assets/models/new_building/roof_supports.fbx", 0.005);
	
	building:add_model("assets/models/new_building/building.fbx", 0.005);
	
	floor:add_model("assets/models/new_building/floor.fbx", 0.005);
	
	alcove_1:add_model("assets/models/new_building/alcove1.fbx", 0.005);
	alcove_2:add_model("assets/models/new_building/alcove2.fbx", 0.005);

	glass_panel1:add_model("assets/models/new_building/glass_panel1.fbx", 0.005);
	glass_panel2:add_model("assets/models/new_building/glass_panel2.fbx", 0.005);

	roof:get_transform().pos = Vector3.new(0.0, 7.25, 0.0);
	roof:get_transform().scale = Vector3.new(1.0, 1.0, 1.0);

	roof_support:get_transform().pos = roof:get_transform().pos - Vector3.new(0.0, 0.5, 0.0);
	
	building:get_transform().pos = Vector3.new(2.0, -1.0, 9.295);
	building:get_transform().rot = Vector3.new(-90.0, 0.0, 0.0);
	building:get_transform().scale = Vector3.new(28.415, 1.0, 1.0);

	floor:get_transform().pos = Vector3.new(-1.0, -1.251, 1.0);
	floor:get_transform().rot = Vector3.new(0.0, 0.0, 0.0);
	floor:get_transform().scale = Vector3.new(30.0, 1.0, 30.0);

	alcove_1:get_transform().pos = Vector3.new(-10.0, -1.0, 28.90);
	alcove_1:get_transform().scale = Vector3.new(0.85, 1.0, 0.85);

	alcove_2:get_transform().pos = Vector3.new(-12.65, -1.25, 27.90);
	alcove_2:get_transform().scale = Vector3.new(0.90, 1.0, 0.95);

	glass_panel1:get_transform().pos = Vector3.new(-10.675, -1.0, 9.35);
	glass_panel1:get_transform().rot = Vector3.new(-90.0, 0.0, 0.0);

	glass_panel2:get_transform().pos = Vector3.new(-10.675, -1.0, 9.25);
	glass_panel2:get_transform().rot = Vector3.new(-90.0, 0.0, 0.0);
end

function update()
end