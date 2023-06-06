function init()
    scene:create_entity("Player", "entities/player/player.lua")
    scene:create_entity("terrain", "entities/terrain/physicsTerrain.lua");
    
    -- scene:create_entity("Bob", "entities/agents/bob.lua");
    -- scene:create_entity("Rabbit", "entities/agents/rabbit.lua");

    local rabbits = {};
    for i = 1,2 do
        rabbits[i] = scene:create_entity("Rabbit" .. i, "entities/agents/rabbit.lua");
        rabbits[i]:get_transform().pos = Vector3.new(10.0 * i, 0.0, -2.0);
    end

    -- scene:create_entity("Bears", "entities/agents/bear.lua")
    -- scene:create_entity("Rabbits", "entities/agents/rabbit.lua");
    -- scene:create_entity("Deers", "entities/agents/deer.lua");
    -- scene:create_entity("Wolves", "entities/agents/wolf.lua");

end

function update()
    --GUI:start_window("FPS Window", 240.0, 240.0, 10.0, 10.0);

    --local fps = GUI:get_fps()
    --GUI:add_text("GAME FPS: " .. Time.fps, false, false, 1.0, 1.0, 1.0, 1.0, 1.0);

    --GUI:end_window();

    if(Input:get_key_down(KEYS["KEY_ESC"])) then
        toggle_wireframe();
        toggle_skybox();
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