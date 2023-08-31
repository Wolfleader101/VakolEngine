function init()
    scene:create_entity("Player", "entities/player/noclip.lua")

    -- scene:create_entity("Digital Twin", "entities/test/digital_twin.lua");

    local path = "coreAssets/textures/Skybox/";
	local extension = ".png";

    local faces = {
                    path .. "right"   .. extension, 
                    path .. "left"  .. extension, 
					path .. "top"    .. extension, 
					path .. "bottom" .. extension, 
					path .. "front"  .. extension, 
					path .. "back"   .. extension};

	scene:generate_skybox(faces);
end

function update()

    if (Input:get_key_down(KEYS["KEY_ESC"])) then
        change_scene("test menu");
    end
    
    GUI:start_window("Stats", false, false, 0.0, 0.0 , 25, 25); 

    GUI:add_text("GAME FPS: " .. Time.fps, false, false, 1.0, 0.0, 1.0, 0.0, 1.0);

    GUI:end_window();
end

function tick()
end