function init()
    print("agents scene");
    scene:create_entity("terrain", "entities/terrain/physicsTerrain.lua");
    --scene:create_entity("CLOD Terrain", "entities/terrain/CLODTerrain.lua");

    scene:create_entity("Player Camera Controller", "entities/player/cameraController.lua");

    scene:create_entity("instances", "entities/instancing.lua");
    
    --scene:create_entity("Jeff", "entities/agents/jeff.lua");
    --scene:create_entity("Bob", "entities/agents/bob.lua");
end


function update()
    GUI:start_window("Agents Window", 240.0, 240.0, 10.0, 10.0);

    GUI:add_text("GAME FPS: " .. Time.fps, false, false, 1.0);
    --GUI:add_text("Bob State:  " .. scene.globals.bobState:get_state());
    GUI:add_text("Key '0' " .. " Pause Animation", false, false, 1.0);
    GUI:add_text("Key '9' " .. " Reset Animation", false, false, 1.0);

    GUI:end_window();
end