function init(scene)
    -- scene:create_entity("spinning Cube", "entities/spinning_cube.lua");
    scene:create_entity("terrain", "entities/terrain.lua"); -- switch between basic or advanced_example
    scene:create_entity("Camera Controller", "entities/cameraController.lua");
end


function update(scene)
    GUI:start_window("FPS Window", 240.0, 240.0, 10.0, 10.0);

    local fps = GUI:get_fps()
    GUI:add_text("GUI FPS: " .. math.floor(fps));
    GUI:add_text("GAME FPS: " .. Time.fps);
    GUI:add_text("Delta Time:  " .. string.format("%.4f", Time.delta_time));

    GUI:end_window();
end