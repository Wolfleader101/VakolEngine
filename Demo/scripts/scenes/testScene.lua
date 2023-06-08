function init()
    scene:create_entity("Player", "entities/player/player.lua")
    scene:create_entity("terrain", "entities/terrain/physicsTerrain.lua");

    scene:create_entity("serialization", "entities/misc/serialization.lua");

    local rabbits = {};
    local birds  = {};
    scene.globals.monsters = {};    
    scene.globals.bears = {};
    
    for i = 1, 5 do
        scene.globals.bears[i] = scene:create_entity("Bear" .. i, "entities/agents/bear.lua");
        scene.globals.bears[i]:get_transform().pos = Vector3.new(5.0 * i, 0.0, -2.0);
    end

    scene:create_entity("Test", "entities/physics_example.lua")

    -- for i = 1, 5 do
    --     scene.globals.monsters[i] = scene:create_entity("Monster" .. i, "entities/agents/monster.lua");
    --     scene.globals.monsters[i]:get_transform().pos = Vector3.new(5.0 * i, 0.0, -2.0);
    -- end

    -- for i= 1,5 do
    --     birds[i] = scene:create_entity("Bird" .. i, "entities/agents/bird.lua");
    --     birds[i]:get_transform().pos = Vector3.new(5.0 * i, 0.0, 2.0);
    -- end

--     for i = 1, 5 do
--         rabbits[i] = scene:create_entity("Rabbit" .. i, "entities/agents/rabbit.lua");
--         rabbits[i]:get_transform().pos = Vector3.new(5.0 * i, 0.0, -2.0);
--     end
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

end