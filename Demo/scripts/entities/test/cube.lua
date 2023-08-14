function init()
	entity:generate_model("coreAssets/models/cube.obj", 0.5);

	entity:get_transform().pos = Vector3.new(0.0, 1.0, 0.0);
end

function update()
end