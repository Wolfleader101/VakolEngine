function init(scene, entity)
    local model = entity:add_model("coreAssets/models/kiki_resized.obj") -- get model and add a drawable component
    --local body_texture = create_texture("coreAssets/textures/kiki_body.jpg");
    
    model:set_shader("coreAssets/shaders/kiki.prog") -- set the shader on the model (automatically binds it)

    local mesh = model:get_mesh(0);
    local material = mesh:get_material();

    material:add_texture(load_texture("coreAssets/textures/kiki_body.jpg"));
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