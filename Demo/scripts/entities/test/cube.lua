function init()
	entity:add_model("coreAssets/models/cube.obj", 1);

	entity:add_rigid();
	entity:add_box_collider(Vector3.new(1.0, 1.0, 1.0));
end

function update()
end