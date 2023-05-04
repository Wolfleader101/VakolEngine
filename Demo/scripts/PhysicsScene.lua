function init(scene)
    scene:create_entity("physics plane", "physics_plane.lua");
    --scene:create_entity("physics example", "physics_example.lua");
    scene:create_entity("Camera Controller", "cameraController.lua");
end


function update(scene)
    GUI:start_window("FPS Window", 240.0, 240.0, 10.0, 10.0);

    local fps = GUI:get_fps()
    GUI:add_text("GUI FPS: " .. math.floor(fps));
    GUI:add_text("GAME FPS: " .. Time.fps);
    GUI:add_text("Delta Time:  " .. string.format("%.4f", Time.delta_time));

    GUI:end_window();
end