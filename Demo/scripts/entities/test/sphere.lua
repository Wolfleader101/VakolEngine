function init()
	local model = entity:add_model("coreAssets/models/sphere.obj", 1.0);

	--local material = model:get_material(0);
	--local shader = material:get_shader();

	--material:set_diffuse_color(Vector3.new(1.0, 1.0, 0.0));

	entity:get_transform().pos = Vector3.new(0.0, 0.0, 0.0);
	entity:get_transform().rot = Vector3.new(0.0, 0.0, 0.0);
    entity:add_rigid();
	entity:add_sphere_collider(1.0);
	
end

function update()
end