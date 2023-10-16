INTERACTABLE = nil;

function init()
	entity:add_model("coreAssets/models/cube.obj", 1);
	entity:get_transform().pos = Vector3.new(0.0, 2.0, -5.0);

	entity:add_script("throwable", "components/throwable.lua");
	local throwableScript = entity:get_script("throwable");
	INTERACTABLE = throwableScript.THROWABLE;
end

function update()

end

function phys_update()
	-- local rb = entity:get_rigid();
	-- if (Input:get_key(KEYS["KEY_1"])) then
    --     rb:add_force(Vector3.new(0.0, 0.0, 100.0));
    -- end
end