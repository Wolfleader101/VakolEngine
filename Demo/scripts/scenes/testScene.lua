function init()
    scene:create_entity("Camera Controller", "entities/player/cameraController.lua");
    --scene:create_entity("spinning Cube", "entities/spinning_cube.lua");
    --scene:create_entity("instancing example", "entities/instancing.lua");
    --scene:create_entity("kiki", "entities/kiki_test.lua");
    --scene:create_entity("animated model", "entities/test_animation.lua");
    --scene:create_entity("terrain", "entities/terrain/terrain.lua");
    scene:create_entity("terrain", "entities/terrain/physicsTerrain.lua");
end

function update()
    GUI:start_window("FPS Window", 240.0, 240.0, 10.0, 10.0);

    local fps = GUI:get_fps()
    GUI:add_text("GUI FPS: " .. math.floor(fps), false, false, 1.0);
    GUI:add_text("GAME FPS: " .. Time.fps, false, false, 1.0);
    GUI:add_text("Delta Time:  " .. string.format("%.4f", Time.delta_time), false, false, 1.0);

    GUI:add_text(" ", false, false, 1.0);

    GUI:add_text("CONTROLS:", false, false, 1.0);

    GUI:add_text("W - Move left", false, false, 1.0);
    GUI:add_text("A - Move forward", false, false, 1.0);
    GUI:add_text("S - Move left", false, false, 1.0);
    GUI:add_text("D - Move right", false, false, 1.0);

    GUI:add_text("MOUSE - Move camera", false, false, 1.0);


    GUI:add_text("K - Toggle wireframe", false, false, 1.0);
    GUI:add_text("ESC - Menu", false, false, 1.0);

    GUI:end_window();

    if(Input:get_key_down(KEYS["KEY_ESC"])) then
        scene:set_active(false);

        local menu = get_scene("Start Scene");
        menu:set_active(true);
    end

    if(Input:get_key_down(KEYS["KEY_5"])) then 
        scene:serialize("assets/scenes");
        print("Checkpoint saved!")
    end

    if(Input:get_key_down(KEYS["KEY_6"])) then
        scene:deserialize("assets/scenes/Test Scene");
	end

end