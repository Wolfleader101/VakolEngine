function init(scene)
    scene:create_entity("cameraController.lua");
    scene:create_entity("player.lua");
end


function update(scene)
    GUI:start_window("FPS Window", 240.0, 240.0, 10.0, 10.0);

    local fps = GUI:get_fps()
    GUI:add_text("GUI FPS: " .. math.floor(fps));
    GUI:add_text("GAME FPS: " .. math.floor(1000 / Time.delta_time));

    GUI:end_window();
end