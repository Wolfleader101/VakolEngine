function init(scene)
    -- scene:create_entity("spinning Cube", "entities/spinning_cube.lua");
    scene:create_entity("terrain", "entities/terrain.lua"); -- switch between basic or advanced_example
    scene:create_entity("Camera Controller", "entities/cameraController.lua");

    for i=1,15 do
        local cube = scene:create_entity("spinning Cube " .. i, "");
        local terr_ent = scene:get_entity("terrain");
        local terrain = terr_ent:get_terrain();
        local terr_scale = terr_ent:get_transform().scale;
        local x = math.random(0, terrain:get_size() * terr_scale.x);
        local z = math.random(0, terrain:get_size() * terr_scale.z);

        local y = (terrain:get_height(x / terr_scale.x, z / terr_scale.z) * terr_scale.y) + 5;

        cube:get_transform().pos.x = x;
        cube:get_transform().pos.y = y;
        cube:get_transform().pos.z = z;

        local model = cube:add_model("coreAssets/models/cube.obj") -- get model and add a drawable component
        model:set_shader("coreAssets/shaders/basic.prog") -- set the shader on the model (automatically binds it)

        local diffuse_map = texture("coreAssets/textures/pisikek.png");

        local shader = model:get_shader(); -- get the shader from the model
        shader:set_int("texture_diffuse", 0);                   -- in this case, we set a uniform variable "tint" of type vec3 to red

        diffuse_map:bind_texture(0);
    end
end


function update(scene)
    GUI:start_window("FPS Window", 240.0, 240.0, 10.0, 10.0);

    local fps = GUI:get_fps()
    GUI:add_text("GUI FPS: " .. math.floor(fps));
    GUI:add_text("GAME FPS: " .. Time.fps);
    GUI:add_text("Delta Time:  " .. string.format("%.4f", Time.delta_time));

    GUI:end_window();
end