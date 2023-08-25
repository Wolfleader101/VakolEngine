function init()
    scene:create_entity("Player", "entities/player/player.lua")
    scene:create_entity("terrain", "entities/terrain/physicsTerrain.lua");

    --scene:create_entity("serialization", "entities/misc/serialization.lua");
    
    --scene:create_entity("Bob", "entities/agents/bob.lua");
    --scene:create_entity("Rabbit", "entities/agents/rabbit.lua");
    --scene:create_entity("Rabbit Test", "entities/agents/rabbit_test.lua");
    
    local birds = {};
    local rabbits = {};
    
    scene:create_entity("Test", "entities/physics_example.lua")

    for i = 1, 5 do
         scene.globals.monsters[i] = scene:create_entity("Monster" .. i, "entities/agents/monster.lua");
         scene.globals.monsters[i]:get_transform().pos = Vector3.new(5.0 * i, 0.0, -2.0);
    end

    for i= 1,5 do
         birds[i] = scene:create_entity("Bird" .. i, "entities/agents/bird.lua");
         birds[i]:get_transform().pos = Vector3.new(5.0 * i, 0.0, 2.0);
    end

    for i = 1, 5 do
       rabbits[i] = scene:create_entity("Rabbit" .. i, "entities/agents/rabbit.lua");
       rabbits[i]:get_transform().pos = Vector3.new(5.0 * i, 0.0, -2.0);
    end
end

function update()
    --GUI:start_window("FPS Window", 240.0, 240.0, 10.0, 10.0);

    --local fps = GUI:get_fps()
    --GUI:add_text("GAME FPS: " .. Time.fps, false, false, 1.0, 1.0, 1.0, 1.0, 1.0);

    --GUI:end_window();

    if(Input:get_key_down(KEYS["KEY_ESC"])) then
        scene:set_active(false);

        toggle_wireframe();
        toggle_skybox();

        local menu = get_scene("Start Scene");
        menu:set_active(true);
    end

    if(Input:get_key_down(KEYS["KEY_5"])) then
        scene:serialize("assets/scenes");
        print("Checkpoint saved!");
    end

    if(Input:get_key_down(KEYS["KEY_6"])) then
        scene:deserialize("assets/scenes/".. scene:get_name());
        print("Checkpoint loaded!")
    end

end