function init()
    print("agents scene");
    scene:create_entity("Fly Cam", "entities/FlyCam.lua");
    
    scene:create_entity("Terrain", "entities/physicsTerrain.lua");

    scene:create_entity("Jeff", "entities/jeff.lua");
    scene:create_entity("Bob", "entities/bob.lua");

    print(scene.globals.bobState.get_state())
end

function update()
    GUI:start_window("Agents Window", 240.0, 240.0, 10.0, 10.0);

    GUI:add_text("Bob State:  " .. scene.globals.bobState:get_state());

    GUI:end_window();
end