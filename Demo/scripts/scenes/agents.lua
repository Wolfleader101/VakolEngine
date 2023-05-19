function init()
    print("agents scene");
    scene:create_entity("Player Camera Controller", "entities/player/cameraController.lua");
    
    scene:create_entity("terrain", "entities/terrain/physicsTerrain.lua");

    scene:create_entity("Jeff", "entities/agents/jeff.lua");
    scene:create_entity("Bob", "entities/agents/bob.lua");

    print(scene.globals.bobState.get_state())
end

function update()
    if (Input:get_key_down(KEYS["KEY_Z"])) then
        print("TEST")
    end

    if (Input:get_key(KEYS["KEY_Z"])) then
        print_err("TEST")
    end
    GUI:start_window("Agents Window", 240.0, 240.0, 10.0, 10.0);

    GUI:add_text("GAME FPS: " .. Time.fps);
    GUI:add_text("Bob State:  " .. scene.globals.bobState:get_state());

    GUI:end_window();

        GUI:start_window("Agents Window", 240.0, 240.0, 250.0, 250.0);

    GUI:add_text("GAME FPS: " .. Time.fps);
    GUI:add_text("Bob State:  " .. scene.globals.bobState:get_state());

    GUI:end_window();
end