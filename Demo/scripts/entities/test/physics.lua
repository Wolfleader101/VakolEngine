function init()
	local floor = scene:create_entity("Floor", "");

	local sphere = scene:create_entity("Sphere", "");
	local cube = scene:create_entity("Cube", "");

	sphere:add_model("coreAssets/models/sphere.obj", 1.0);

	sphere:get_transform().pos.y = 10.0;
	cube:get_transform().pos = Vector3.new(4.0, 5.0, 0.0);

    sphere:add_rigid();
	sphere:add_sphere_collider(1.0);

	floor:add_model("assets/models/digital_twin/GROUND.fbx", 0.02);
	cube:add_model("coreAssets/models/cube.obj", 1.0);
	
	local rb = floor:add_rigid();
	rb.type = BodyType.Static;

	floor:add_box_collider(Vector3.new(20.0, 0.1, 50.0));

	rb = cube:add_rigid();
	cube:add_box_collider(Vector3.new(1.0, 1.0, 1.0));
end

function update()
end