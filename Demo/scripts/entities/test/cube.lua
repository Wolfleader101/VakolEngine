function init()
	entity:add_model("coreAssets/models/cube.obj", 1);

    local rb = entity:add_rigid();
	rb.mass = 1;
	entity:add_box_collider(Vector3.new(1.0, 1.0, 1.0));
end

function phys_update()
	-- local rb = entity:get_rigid();
	-- if (Input:get_key(KEYS["KEY_1"])) then
    --     rb:add_force(Vector3.new(0.0, 0.0, 100.0));
    -- end
end