function init()
	entity:add_model("assets/models/new_building/new_building_roof.fbx", 0.01);

	entity:get_transform().pos = Vector3.new(0.0, 10.0, 0.0);
	entity:get_transform().rot = Vector3.new(0.0, 0.0, 0.0);
end

function update()
end