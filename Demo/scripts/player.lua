function init(scene, entity)
    entity:add_model("coreAssets/models/cube.obj")
end

local function lerp(a, b, t)
    return a + (b - a) * t
end

function update(scene, entity)
    local amplitude = 2.0
    local frequency = 1.0
    local theta = Time.curr_time * frequency
    local dist = amplitude * math.sin(theta)
    local hover = lerp(0, 1, dist)
    entity:get_transform().pos.y = hover

    entity:get_transform().rot.x = entity:get_transform().rot.x + (Time.delta_time * 1.25)
    entity:get_transform().rot.y = entity:get_transform().rot.y + (Time.delta_time * 1.25)
    entity:get_transform().rot.z = entity:get_transform().rot.z + (Time.delta_time * 1.25)

end