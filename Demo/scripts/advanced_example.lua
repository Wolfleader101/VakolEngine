function init(scene, entity)

    print("For the best-looking results with fog, set clear color to VAKOL_FOGGY_2.");
    print("If the fog is too thick, lower the FOG_DENSITY in the .lua file");

    -- local terrain = entity:add_terrain_heightmap("coreAssets/textures/Heightmaps/height_map.raw");
    --local terrain = entity:add_terrain_fault_formation(1024, 128, 0.97, true, -10, 10); -- size, iterations, filter, random, minHeight, maxHeight
    local terrain = entity:add_clod_terrain(256)
    
    local model = terrain:get_model();
    local mesh = model:get_mesh();
    local material = mesh:get_material();
    
    local height_map = raw_texture("coreAssets/textures/HeightMaps/height128.raw");

    local water_layer_1 = texture("coreAssets/textures/Water/water_0.bmp", false, false);
    local water_layer_2 = texture("coreAssets/textures/Water/water_5.bmp", false, false);

    local layer_1 = texture("coreAssets/textures/extras/clover.png", false, false);
    local layer_2 = texture("coreAssets/textures/extras/grass.png", false, false);
    local layer_3 = texture("coreAssets/textures/extras/grass_dark.png", false, false);
    local layer_4 = texture("coreAssets/textures/extras/rocks_2.jpg", false, false);
    local layer_5 = texture("coreAssets/textures/extras/rocks.jpg", false, false);
    local layer_6 = texture("coreAssets/textures/extras/snow.png", false, false);

    material:add_texture(height_map);
    
    material:add_texture(water_layer_1);
    material:add_texture(water_layer_2);

    material:add_texture(layer_1);
    material:add_texture(layer_2);

    material:add_texture(layer_3);
    material:add_texture(layer_4);

    material:add_texture(layer_5);
    material:add_texture(layer_6);

    
    -- local model = entity:add_model("coreAssets/models/cornellBox.obj") -- get model and add a drawable component
    model:set_shader("coreAssets/shaders/clod_terrain.prog") -- set the shader on the model (automatically binds the shader)
    local shader = model:get_shader();                            -- get the shader from the model

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

    shader:set_bool("enable_fog", true);
    shader:set_float("FOG_DENSITY", 0.0);

    shader:set_bool("enable_water", true);

    -- shader:set_vec3("light.position", 0.0, 0.5, 7.5);
    -- shader:set_vec3("light.direction", math.rad(0.0), math.rad(-15.0), math.rad(-90.0));

    -- option choices
    -- 0 = directional lighting (sunlight)
    -- 1 = point light (room light)
    -- 2 = spot light (flashlight)
end

local function lerp(a, b, t)
    return a + (b - a) * t
end

function update(scene, entity)
    -- local amplitude = 2.0
    -- local frequency = 1.0
    -- local theta = Time.curr_time * frequency
    -- local dist = amplitude * math.sin(theta)
    -- local hover = lerp(0, 1, dist)
    -- entity:get_transform().pos.y = hover

    -- entity:get_transform().rot.x = entity:get_transform().rot.x + (Time.delta_time * 1.25)
    -- entity:get_transform().rot.y = entity:get_transform().rot.y + (Time.delta_time * 1.25)
    -- entity:get_transform().rot.z = entity:get_transform().rot.z + (Time.delta_time * 1.25)

end