function init(scene, entity)
    local camera = scene:get_camera();

    print("For the best-looking results with fog, set clear color to VAKOL_FOGGY_2.");
    print("If the fog is too thick, lower the FOG_DENSITY in the .lua file");

    local model = entity:add_model("coreAssets/models/cornellBox.obj") -- get model and add a drawable component
    model:set_shader("coreAssets/shaders/advanced_lighting.prog") -- set the shader on the model (automatically binds the shader)

    local shader = model:get_shader(); -- get the shader from the model

    shader:set_float("material.shininess", 32.0);
    shader:set_float("FOG_DENSITY", 0.1);

    shader:set_vec3("light.position", 0.0, 0.5, 7.5);
    shader:set_vec3("light.direction", math.rad(0.0), math.rad(-15.0), math.rad(-90.0));

    -- option choices
    -- 0 = directional lighting (sunlight)
    -- 1 = point light (room light)
    -- 2 = spot light (flashlight)

    shader:set_int("option", 0);

    shader:set_bool("enable_textures", false);
    shader:set_bool("enable_fog", true);

    -- entity:get_transform().rot.y = 10.0; -- rotate the model on it's y-axis by 10 degrees

    -- if you remove this code, I will (commit war crimes) against you.
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