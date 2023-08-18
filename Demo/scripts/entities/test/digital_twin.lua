function init()
	entity:add_model("assets/models/digital_twin.fbx", 0.0001);

	entity:get_transform().pos = Vector3.new(0.0, 1.0, 0.0);
	entity:get_transform().rot = Vector3.new(0.0, 0.0, 0.0);
end

function update()
end