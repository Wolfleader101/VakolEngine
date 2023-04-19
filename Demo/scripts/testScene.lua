function init(scene)
    scene:create_entity("cameraController.lua");
    scene:create_entity("player.lua");
end


function update(scene)
    GUI:start_window_creation("FPS Window", 240.0, 240.0, 10.0, 10.0);

    GUI:add_text("Test String");

    GUI:end_window_creation();
    -- print("Hello from TestScene update!!!");
    -- print(Time["delta_time"]);
end