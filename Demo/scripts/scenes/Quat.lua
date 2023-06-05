function init()
    --scene:create_entity("plane", "entities/physics_plane.lua");
    
    scene:create_entity("Camera", "entities/DebugCam.lua");
    
    scene:create_entity("cube", "entities/physics_example.lua");
    
    scene:enable_debug(true);
end



function update()
    if(Input:get_key_down(KEYS["KEY_ESC"])) then
        app_run(false);
    end
end

