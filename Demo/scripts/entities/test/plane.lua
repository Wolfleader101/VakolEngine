function init()

	entity:add_model("coreAssets/models/plane.obj", 100.0);

	entity:get_transform().pos = Vector3.new(0.0, -0.001, 0.0);
	entity:get_transform().rot = Vector3.new(0.0, 0.0, 0.0);

	--entity:replace_texture("coreAssets/textures/white.png", "assets/textures/grass.png", "DIFFUSE");
end

function update()
end