function init()
	entity:generate_model("coreAssets/models/sphere.obj", 0.5);

	entity:get_transform().pos = Vector3.new(-2.0, -0.5, 0.0);
end

function update()
end