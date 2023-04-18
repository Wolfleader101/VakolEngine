function init(scene, entity)
    local camera = scene:get_camera();

    print(camera:get_pos().z);

    local model = entity:add_model("coreAssets/models/cube.obj") -- get model and add a drawable component
    model:set_shader("coreAssets/shaders/basic.prog") -- set the shader on the model (automatically binds it)

    local shader = model:get_shader(); -- get the shader from the model
    shader:set_vec3("tint", 1.0, 0.0, 0.0); -- in this case, we set a uniform variable "tint" of type vec3 to red

    entity:get_transform().rot.y = 10.0; -- rotate the model on it's y-axis by 10 degrees

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