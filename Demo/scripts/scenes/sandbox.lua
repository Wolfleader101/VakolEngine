function init()
    scene:create_entity("Player", "entities/player/noclip.lua")

    --scene:create_entity("Digital Twin", "entities/test/digital_twin.lua");

    scene:create_entity("290 World", "entities/worlds/290_world.lua");
    

    local cube = scene:create_entity("Cube", "entities/test/cube.lua");
    
    -- local sphere = scene:create_entity("Sphere", "entities/test/sphere.lua");
    --sphere:get_transform().pos = Vector3.new(0.0, 1.5, 0.0);
    cube:get_transform().pos = Vector3.new(3.0, 1.5, 0.0);

    local path = "coreAssets/textures/Skybox/";
	local extension = ".png";

	local faces = { path .. "right"  .. extension, 
					path .. "left"   .. extension, 
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