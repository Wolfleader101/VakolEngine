function init(scene)
    --scene:create_entity("physics plane", "entities/physics_plane.lua");
    --scene:create_entity("physics example", "entities/physics_example.lua");
    
    
    scene:create_entity("Fly Cam", "entities/FlyCam.lua");
    
    scene:create_entity("Terrain", "entities/physicsTerrain.lua");
    

    scene:create_entity("Sphere", "entities/sphere.lua");
    

    local phy = scene:get_physics();


    phy:enable_debug(true);
end


function update(scene)
    GUI:start_window("FPS Window", 240.0, 240.0, 10.0, 10.0);

    local fps = GUI:get_fps()
    GUI:add_text("GUI FPS: " .. math.floor(fps));
    GUI:add_text("GAME FPS: " .. Time.fps);
    GUI:add_text("Delta Time:  " .. string.format("%.4f", Time.delta_time));

    GUI:end_window();
end