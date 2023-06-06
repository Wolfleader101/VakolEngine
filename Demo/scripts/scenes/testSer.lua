function init()
    
    scene:create_entity("Serialize", "entities/misc/serialization.lua");
    scene:create_entity("Start Terrain", "entities/terrain/startTerrain.lua");
    scene:create_entity("Fly Camera", "entities/FlyCam.lua");
    
    
end


function update()

    if(Input:get_key_down(KEYS["KEY_ESC"])) then
        app_run(false);
    end
end
