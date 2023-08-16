function init()
	entity:generate_model("coreAssets/models/cube.obj", 0.5);

	entity:get_transform().pos = Vector3.new(2.0, -0.5, 0.0);
	entity:get_transform().rot = Vector3.new(0.0, 140.0, 0.0);
end

function update()
end