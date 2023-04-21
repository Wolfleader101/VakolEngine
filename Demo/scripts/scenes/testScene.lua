function init(scene)
    -- scene:create_entity("spinning Cube", "entities/spinning_cube.lua");
    scene:create_entity("terrain", "entities/terrain.lua"); -- switch between basic or advanced_example
    scene:create_entity("Camera Controller", "entities/cameraController.lua");

    for i=1,30 do
        local kiki = scene:create_entity("kiki_model " .. i, "");

        kiki:get_transform().scale.x = 5;
        kiki:get_transform().scale.y = 5;
        kiki:get_transform().scale.z = 5;

        local terr_ent = scene:get_entity("terrain");
        local terrain = terr_ent:get_terrain();
        local terr_scale = terr_ent:get_transform().scale;

        local x = math.random(0, terrain:get_size()) * 10;
        local z = math.random(0, terrain:get_size()) * 10;
        local y = (terrain:get_height(x / terr_scale.x, z / terr_scale.z) * terr_scale.y) + 5;

        kiki:get_transform().pos.x = x;
        kiki:get_transform().pos.y = y;
        kiki:get_transform().pos.z = z;

        local _model = kiki:add_model("coreAssets/models/kiki_resized.obj") -- get model and add a drawable component

        local _mesh = _model:get_mesh();

        local _material = _mesh:get_material();

        _model:set_shader("coreAssets/shaders/basic.prog") -- set the shader on the model (automatically binds it)

        local _diffuse = texture("coreAssets/textures/kiki_body.jpg", false, false);

        _material:add_texture(_diffuse);
        _diffuse:bind_texture(97);


        local shader = _model:get_shader(); -- get the shader from the model
        shader:set_int("texture_diffuse", 97); -- in this case, we set a uniform variable "tint" of type vec3 to red

    end
end



function update(scene)
    GUI:start_window("FPS Window", 240.0, 240.0, 10.0, 10.0);

    local fps = GUI:get_fps()
    GUI:add_text("GUI FPS: " .. math.floor(fps));
    GUI:add_text("GAME FPS: " .. Time.fps);
    GUI:add_text("Delta Time:  " .. string.format("%.4f", Time.delta_time));

    GUI:add_text(" ");

    GUI:add_text("CONTROLS:");

    GUI:add_text("W - Move left");
    GUI:add_text("A - Move forward");
    GUI:add_text("S - Move left");
    GUI:add_text("D - Move right");

    GUI:add_text("MOUSE - Move camera");


    GUI:add_text("K - Toggle wireframe");
    GUI:add_text("ESC - Exit");

    GUI:end_window();
end