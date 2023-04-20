function init(scene, entity)
    local model = entity:add_model("coreAssets/models/cube.obj");
    local mesh = model:get_mesh();
    local material = mesh:get_material();

    entity:get_transform().scale.x = 10;

    local water_layer_1 = texture("coreAssets/textures/Water/water_0.bmp", false, false);
    local water_layer_2 = texture("coreAssets/textures/Water/water_5.bmp", false, false);

    material:add_texture(water_layer_1);
    material:add_texture(water_layer_2);

    model:set_shader("coreAssets/shaders/water.prog") -- set the shader on the model (automatically binds the shader)
    local shader = model:get_shader();

    shader:set_int("texture_0", 0);
    shader:set_int("texture_1", 1);
end

function update(scene, entity)
end