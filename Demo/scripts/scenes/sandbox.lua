function init()
    print("sss");
    scene:create_entity("Player", "entities/player/noclip.lua")
    scene:create_entity("terrain", "entities/terrain/sandboxTerrain.lua");

    for i = 1, 20, 1 do   
    scene:create_entity("Bear", "entities/agents/sandboxbear.lua")
    end
end

function update()

    if (Input:get_key_down(KEYS["KEY_ESC"])) then
        set_current_scene("test menu");
    end
    
    GUI:start_window("Stats", false, false, 0.0, 0.0 , 25, 25); 

    GUI:add_text("GAME FPS: " .. Time.fps, false, false, 1.0, 0.0, 1.0, 0.0, 1.0);

    GUI:end_window();
end

function tick()
end