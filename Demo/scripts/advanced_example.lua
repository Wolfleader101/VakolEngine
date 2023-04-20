function init(scene, entity)

    print("For the best-looking results with fog, set clear color to VAKOL_FOGGY_2.");
    print("If the fog is too thick, lower the FOG_DENSITY in the .lua file");

    -- local terrain = entity:add_terrain_heightmap("coreAssets/textures/Heightmaps/height_map.raw");
    --local terrain = entity:add_terrain_fault_formation(1024, 128, 0.97, true, -10, 10); -- size, iterations, filter, random, minHeight, maxHeight
    local terrain = entity:add_clod_terrain(128)
    
    local model = terrain:get_model();
    local mesh = model:get_mesh();
    local material = mesh:get_material();
    
    local height_map = raw_texture("coreAssets/textures/HeightMaps/height128.raw");

    local water_diffuse = texture("coreAssets/textures/Water/water_0.bmp", false, false);
    local water_foam = texture("coreAssets/textures/Water/water_1.jpg", false, false);

    local dirt_layer = texture("coreAssets/textures/dirt_2.jpg", true, false);
    local grass_layer = texture("coreAssets/textures/grass.png", true, true);
    local rock_layer = texture("coreAssets/textures/rocky.jpg", true, false)

    material:add_texture(height_map);
    
    material:add_texture(water_diffuse);
    material:add_texture(water_foam);

    material:add_texture(dirt_layer);
    material:add_texture(grass_layer);
    material:add_texture(rock_layer);


    height_map:bind_texture(0);
    
    -- local model = entity:add_model("coreAssets/models/cornellBox.obj") -- get model and add a drawable component
    model:set_shader("coreAssets/shaders/clod_terrain.prog") -- set the shader on the model (automatically binds the shader)
    local shader = model:get_shader();                            -- get the shader from the model

    shader:set_int("height_map", 0);

    shader:set_int("water_diffuse", 1);
    shader:set_int("water_foam", 2);

    shader:set_int("dirt_layer", 3);
    shader:set_int("grass_layer", 4);
    shader:set_int("rock_layer", 5);
    shader:set_int("snow_layer", 6);

    water_diffuse:bind_texture(1);
    water_foam:bind_texture(2);

    dirt_layer:bind_texture(3);
    grass_layer:bind_texture(4);
    rock_layer:bind_texture(5);
    snow_layer:bind_texture(6);

    -- shader:set_vec3("light.position", 0.0, 0.5, 7.5);
    -- shader:set_vec3("light.direction", math.rad(0.0), math.rad(-15.0), math.rad(-90.0));

    -- option choices
    -- 0 = directional lighting (sunlight)
    -- 1 = point light (room light)
    -- 2 = spot light (flashlight)

    -- entity:get_transform().rot.y = 10.0; -- rotate the model on it's y-axis by 10 degrees

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