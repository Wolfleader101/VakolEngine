
function init()
	entity:add_model("coreAssets/models/cube.obj", 1);
	entity:get_transform().pos = Vector3.new(0.0, 10.0, -7.5);

    local rb = entity:add_rigid();

    rb.mass = 1.0;
    rb.type = BodyType.Static;

    entity:add_box_collider(entity:get_transform().scale);
end

function update()

end

function phys_update()
	-- local rb = entity:get_rigid();
	-- if (Input:get_key(KEYS["KEY_1"])) then
    --     rb:add_force(Vector3.new(0.0, 0.0, 100.0));
    -- end
end