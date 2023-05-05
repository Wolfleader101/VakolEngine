function init(scene, entity)
    local model = entity:add_model("coreAssets/models/kiki_resized.obj") -- get model and add a drawable component
    model:set_shader("coreAssets/shaders/kiki.prog") -- set the shader on the model (automatically binds it)

    print("kikiiiii");
    -- local shader = model:get_shader(); -- get the shader from the model
    -- shader:set_vec3("tint", 0.5, 0.2, 0.3); -- in this case, we set a uniform variable "tint" of type vec3 to red
end

-- local function lerp(a, b, t)
--     return a + (b - a) * t
-- end

function update(scene, entity)
    -- local amplitude = 2.0
    -- local frequency = 1.0
    -- local theta = Time.curr_time * frequency
    -- local dist = amplitude * math.sin(theta)
    -- local hover = lerp(5, 10, dist)
    -- entity:get_transform().pos.y = hover

    -- entity:get_transform().rot.x = entity:get_transform().rot.x + (Time.delta_time * 1.25)
    -- entity:get_transform().rot.y = entity:get_transform().rot.y + (Time.delta_time * 1.25)
    -- entity:get_transform().rot.z = entity:get_transform().rot.z + (Time.delta_time * 1.25)

end