function init(scene, entity)
    local AMOUNT = 10;
    local matrices = vector_mat4();

    local model = entity:add_model("coreAssets/models/cube.obj", 1.0, false) -- get model and add a drawable component
    model:set_shader("coreAssets/shaders/instance.prog") -- set the shader on the model (automatically binds it)
    
    local shader = model:get_shader(); -- get the shader from the model

    for i = 1, AMOUNT do
        local mdl_m = Matrix4x4.new(1.0);

        local pos = Vector3.new(1 * i, 0.0, 0.0);
        local scl = Vector3.new(1.0);

        mdl_m = translate(mdl_m, pos);
        mdl_m = scale(mdl_m, scl);

        mdl_m = rotate(mdl_m, math.rad(0.0), Vector3.new(1.0, 0.0, 0.0));
        mdl_m = rotate(mdl_m, math.rad(0.0), Vector3.new(0.0, 1.0, 0.0));
        mdl_m = rotate(mdl_m, math.rad(0.0), Vector3.new(0.0, 0.0, 1.0));

        matrices:add(mdl_m);
    end

    instantiate_model(model, matrices, AMOUNT);

    shader:set_int("diffuse_map", 0);
end

function update(scene, entity)

end