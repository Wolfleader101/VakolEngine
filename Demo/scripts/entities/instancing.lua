function init()
    local AMOUNT = 100;
    local matrices = vector_mat4(AMOUNT);

    print_warn("LOOK AT LUA ACCESS INSTANCE FUNCTION FOR INSTANCING ANIMATED MODELS!!");

    local model = entity:add_model("coreAssets/models/cube.obj", 0.1, false) -- get model and add a drawable component
    model:set_shader("coreAssets/shaders/instance.prog") -- set the shader on the model (automatically binds it)
    
    local shader = model:get_shader(); -- get the shader from the model

    for i = 1, AMOUNT do
        local mdl_m = Matrix4x4.new(1.0);

        local scl = Vector3.new(1.0);
        local pos = Vector3.new(math.random(-64.0, 64.0), 0.0, math.random(-64.0, 64.0));

        local terr_scale = scene.globals.terrain.transform.scale;
        pos.y = (scene.globals.terrain.terr:get_height(pos.x / terr_scale.x, pos.z / terr_scale.z) * terr_scale.y) + 0.015;

        mdl_m = translate(mdl_m, pos);

        mdl_m = rotate(mdl_m, math.rad(0.0), Vector3.new(1.0, 0.0, 0.0));
        mdl_m = rotate(mdl_m, math.rad(math.random(-90.0, 90.0)), Vector3.new(0.0, 1.0, 0.0));
        mdl_m = rotate(mdl_m, math.rad(0.0), Vector3.new(0.0, 0.0, 1.0));

        mdl_m = scale(mdl_m, scl);

        matrices[i] = mdl_m;
    end

    instantiate_model(model, matrices, AMOUNT);

    shader:set_int("diffuse_map", 0);
end

function update()

end