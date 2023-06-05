function init()
  
    local model = entity:add_model("coreAssets/models/cube.obj", 1.0, false, true) -- get model and add a drawable component
    entity:set_shader("coreAssets/shaders/basic.prog") -- set the shader on the model (automatically binds it)
    
    local shader = model:get_shader(); -- get the shader from the model

    shader:set_int("diffuse_map", 0);

    shader:set_vec3("tint", 1.0, 1.0, 1.0); -- in this case, we set a uniform variable "tint" of type vec3 to velvet red
end

local function lerp(a, b, t)
    return a + (b - a) * t
end

function update()
    local amplitude = 2.0
    local frequency = 1.0

    local theta = Time.curr_time * frequency
    local dist = amplitude * math.sin(theta)
    local hover = lerp(0, 1, dist)

    entity:get_transform().pos.y = hover

end