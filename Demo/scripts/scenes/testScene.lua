function init(scene)
    scene:create_entity("Camera Controller", "entities/cameraController.lua");
    --scene:create_entity("spinning Cube", "entities/spinning_cube.lua");
    --scene:create_entity("instancing example", "entities/instancing.lua");
    --scene:create_entity("kiki", "entities/kiki_test.lua");
    scene:create_entity("animated model", "entities/test_animation.lua");
    --scene:create_entity("terrain", "entities/terrain.lua");
end

function update(scene)
    GUI:start_window("FPS Window", 240.0, 240.0, 10.0, 10.0);

    local fps = GUI:get_fps()
    GUI:add_text("GUI FPS: " .. math.floor(fps));
    GUI:add_text("GAME FPS: " .. Time.fps);
    GUI:add_text("Delta Time:  " .. string.format("%.4f", Time.delta_time));

    GUI:add_text(" ");

    GUI:add_text("CONTROLS:");

    GUI:add_text("W - Move left");
    GUI:add_text("A - Move forward");
    GUI:add_text("S - Move left");
    GUI:add_text("D - Move right");

    GUI:add_text("MOUSE - Move camera");


    GUI:add_text("K - Toggle wireframe");
    GUI:add_text("ESC - Exit");

    GUI:end_window();
end