function init()
    scene.globals.waterlevel = 1.0;

    scene:create_entity("Player", "entities/player/player.lua")

    scene:create_entity("terrain", "entities/terrain/physicsTerrain.lua");

    scene:create_entity("serialization", "entities/misc/serialization.lua");

    scene:create_entity("static_props", "entities/instancing_new.lua");

    local rabbits = {};
    local birds  = {};
    scene.globals.monsters = {};    
    scene.globals.bears = {};
    
    for i = 1, 5 do
        scene.globals.bears[i] = scene:create_entity("Bear" .. i, "entities/agents/bear.lua");
        scene.globals.bears[i]:get_transform().pos = Vector3.new(5.0 * i, 0.0, -2.0);
        scene.globals.bears[i]:add_rigid();
    end
  
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
    
    phys_debug = false;

end

function update()
    if (Input:get_key_down(KEYS["KEY_ESC"])) then
        toggle_wireframe();
        toggle_skybox();

        scene:set_active(false);

        local menu = get_scene("Start Scene");
        menu:set_active(true);
    end
    
    if (Input:get_key_down(KEYS["KEY_N"])) then
        phys_debug = not phys_debug;
        scene:enable_debug(phys_debug);
    end
end