function init()

    scene:create_entity("Player", "entities/test/noclip.lua");
    scene:create_entity("Sphere", "entities/test/sphere.lua")
    scene:create_entity("Bear", "entities/test/bear.lua")
    scene:create_entity("Monster", "entities/test/monster.lua")
    --scene:create_entity("Bob", "entities/test/bob.lua")
    scene:create_entity("Cube", "entities/test/cube.lua")

end

function update()

    if (Input:get_key_down(KEYS["KEY_ESC"])) then
        change_scene("test menu");
    end
    
    GUI:start_window("Stats", false, false, 0.0, 0.0 , 25, 25); 

    GUI:add_text("GAME FPS: " .. Time.fps, false, false, 1.0, 0.0, 1.0, 0.0, 1.0);

    GUI:end_window();

end