function init()
    print("Multi Script Root");
    entity:add_script("cube", "entities/test/cube.lua");

    	
    local cube = entity:get_script("cube")
	print(cube.BLAKE)
end

CALEB = "IS GAY"

function phys_update()
	-- local rb = entity:get_rigid();
	-- if (Input:get_key(KEYS["KEY_1"])) then
    --     rb:add_force(Vector3.new(0.0, 0.0, 100.0));
    -- end
end