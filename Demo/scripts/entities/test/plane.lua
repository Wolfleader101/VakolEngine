function init()

	entity:add_model("coreAssets/models/plane.obj", 100.0);

	entity:get_transform().pos = Vector3.new(0.0, -0.001, 0.0);
	entity:get_transform().rot = Vector3.new(0.0, 0.0, 0.0);

	entity:add_texture("assets/textures/grass.png");

end

function update()
end