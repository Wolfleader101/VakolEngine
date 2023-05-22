function init()
    local TREEAMOUNT = 10;
    local matrices = vector_mat4();

    ------------------------------------------------------------------------------------------------
    -- STATIC PROPS
    ------------------------------------------------------------------------------------------------
    -- Trees

    print("Instancing static trees...");

    local model = entity:add_model("coreAssets/models/Imported/OpenGameArt/musdasch/tree.glb", 1.0, false) -- get model and add a drawable component
    model:set_shader("coreAssets/shaders/instance.prog") -- set the shader on the model (automatically binds it)
    
    local shader = model:get_shader(); -- get the shader from the model

    for i = 1, TREEAMOUNT do
        local mdl_m = Matrix4x4.new(1.0);

        local pos = Vector3.new(1 * i, 0.0, 0.0);
        local scl = Vector3.new(1.0);

        mdl_m = translate(mdl_m, pos);
        mdl_m = scale(mdl_m, scl);

        mdl_m = rotate(mdl_m, 0.0, Vector3.new(1.0, 0.0, 0.0));
        mdl_m = rotate(mdl_m, 0.0, Vector3.new(0.0, 1.0, 0.0));
        mdl_m = rotate(mdl_m, 0.0, Vector3.new(0.0, 0.0, 1.0));

        matrices:add(mdl_m);
    end

    entity:instantiate_model(model, matrices, TREEAMOUNT);

    shader:set_vec3v("light.position", Vector3.new(2.0, 2.0, -4.0));

    shader:set_int("material.diffuse_map", 0);
    shader:set_int("material.specular_map", 1);
    shader:set_int("material.normal_map", 2);
end

function update()

end