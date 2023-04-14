function init(scene, entity)
    entity:add_model("coreAssets/models/cube.obj")
    entity:get_transform().scale.x = 1.0
    entity:get_transform().scale.y = 1.0
    entity:get_transform().scale.z = 1.0
end

local function lerp(a, b, t)
    return a + (b - a) * t
end

function update(scene, entity)
    local amplitude = 2.0
    local frequency = 1.0
    local theta = Time.curr_time * frequency
    local dist = amplitude * math.sin(theta)
    local hover = lerp(5, 10, dist)
    entity:get_transform().pos.y = hover

    entity:get_transform().rot.x = entity:get_transform().rot.x + (Time.delta_time * 1.25)
    entity:get_transform().rot.y = entity:get_transform().rot.y + (Time.delta_time * 1.25)
    entity:get_transform().rot.z = entity:get_transform().rot.z + (Time.delta_time * 1.25)

end