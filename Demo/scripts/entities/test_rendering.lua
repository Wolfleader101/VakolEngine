function init()
	--print("hello :)")
end

function update()

	if (Input:get_key(KEYS["KEY_6"])) then
		clear_color(1.0, 0.0, 0.0, 1.0);
	end

	if (Input:get_key(KEYS["KEY_7"])) then
		clear_color(0.0, 1.0, 0.0, 1.0);
	end

	if (Input:get_key(KEYS["KEY_8"])) then
		clear_color(0.0, 0.0, 1.0, 1.0);
	end

	if (Input:get_key_down(KEYS["KEY_9"])) then
		toggle_skybox();
	end

	if (Input:get_key_down(KEYS["KEY_0"])) then
		toggle_wireframe();
	end
end