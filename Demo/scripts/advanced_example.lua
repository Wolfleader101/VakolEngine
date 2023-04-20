function init(scene, entity)

    print("For the best-looking results with fog, set clear color to VAKOL_FOGGY_2.");
    print("If the fog is too thick, lower the FOG_DENSITY in the .lua file");

    -- local terrain = entity:add_terrain_heightmap("coreAssets/textures/Heightmaps/height_map.raw");
    --local terrain = entity:add_terrain_fault_formation(1024, 128, 0.97, true, -10, 10); -- size, iterations, filter, random, minHeight, maxHeight
    local terrain = entity:add_clod_terrain(512)
    
    local model = terrain:get_model();
    local mesh = model:get_mesh();
    local material = mesh:get_material();
    
    local t1 = raw_texture("coreAssets/textures/HeightMaps/test_2.raw");
    material:add_texture(t1);
    t1:bind_texture(0);
    
    -- local model = entity:add_model("coreAssets/models/cornellBox.obj") -- get model and add a drawable component
    model:set_shader("coreAssets/shaders/clod_terrain.prog") -- set the shader on the model (automatically binds the shader)
    local shader = model:get_shader();                            -- get the shader from the model

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