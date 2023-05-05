function init(scene, entity)
    print("Initialising Terrain");
    local terrain = entity:add_clod_terrain("coreAssets/textures/HeightMaps/height128.raw"); -- size

    entity:get_transform().pos.y = -100;
    entity:get_transform().scale.x = 1;
    entity:get_transform().scale.y = 1;
    entity:get_transform().scale.z = 1;

    local model = terrain:get_model();
    local mesh = model:get_mesh();

    local material = mesh:get_material();
    
    local height_map = raw_texture("coreAssets/textures/HeightMaps/height1024.raw");

    local water_layer_1 = texture("coreAssets/textures/Water/water_0.bmp", false, false);
    local water_layer_2 = texture("coreAssets/textures/Water/water_5.bmp", false, false);

    local layer_1 = texture("coreAssets/textures/extras/clover.png", false, false);
    local layer_2 = texture("coreAssets/textures/extras/grass.png", false, false);
    local layer_3 = texture("coreAssets/textures/extras/grass_dark.png", false, false);
    local layer_4 = texture("coreAssets/textures/extras/rocks_2.jpg", false, false);
    local layer_5 = texture("coreAssets/textures/extras/rocks.jpg", false, false);
    local layer_6 = texture("coreAssets/textures/extras/snow.png", false, false);

    material:add_texture(height_map);

    material:add_texture(layer_1);
    material:add_texture(layer_2);

    material:add_texture(layer_3);
    material:add_texture(layer_4);

    material:add_texture(layer_5);
    material:add_texture(layer_6);

    model:set_shader("coreAssets/shaders/clod_terrain.prog") -- set the shader on the model (automatically binds the shader)
    local shader = model:get_shader();                       -- get the shader from the model

    shader:set_int("height_map", 0);

    shader:set_int("water_layer_1", 1);
    shader:set_int("water_layer_2", 2);
    
    shader:set_int("layer_1", 3);
    shader:set_int("layer_2", 4);
    
    shader:set_int("layer_3", 5);
    shader:set_int("layer_4", 6);
    
    shader:set_int("layer_5", 7);
    shader:set_int("layer_6", 8);

    height_map:bind_texture(0);
    
    water_layer_1:bind_texture(1);
    water_layer_2:bind_texture(2);
    
    layer_1:bind_texture(3);
    layer_2:bind_texture(4);
    
    layer_3:bind_texture(5);
    layer_4:bind_texture(6);

    layer_5:bind_texture(7);
    layer_6:bind_texture(8);

    shader:set_bool("enable_fog", false);
    shader:set_float("FOG_DENSITY", 0.025);


    local waterEnt = scene:create_entity("water", "");

    waterEnt:get_transform().scale.x = 1024 * 10;
    waterEnt:get_transform().scale.z = 1024 * 10;
    waterEnt:get_transform().pos.y = -10;

    -- local waterPlane = waterEnt:add_model("coreAssets/models/plane.obj");

    -- local waterMesh = waterPlane:get_mesh();
    -- local waterMaterial = waterMesh:get_material();


    -- waterMaterial:add_texture(water_layer_1);
    -- waterMaterial:add_texture(water_layer_2);

    -- waterPlane:set_shader("coreAssets/shaders/water.prog") -- set the shader on the model (automatically binds the shader)
    -- local waterShader = waterPlane:get_shader();

    -- waterShader:set_int("texture_0", 0);
    -- waterShader:set_int("texture_1", 1);
    shader:set_vec3("light.position", 0.0, 0.5, 7.5);
    shader:set_vec3("light.direction", math.rad(0.0), math.rad(-15.0), math.rad(-90.0));

    print("adding");
    scene:add_terrain_physics(entity);
    print("added");
    
end

function update(scene, entity)

end