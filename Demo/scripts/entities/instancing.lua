function init()
    local TREEAMOUNT = 100;
    local ROCKAMOUNT = 250;
    local WATERLEVEL = 10.0;

    local treeMatrices = vector_mat4(TREEAMOUNT);
    local rockMatrices = vector_mat4(ROCKAMOUNT);

    local tree = entity:add_model("coreAssets/models/Imported/OpenGameArt/Yughues/pine/snow_pine_tree.obj", 0.1, false, false) -- get model and add a drawable component
    entity:set_shader("coreAssets/shaders/instance.prog") -- set the shader on the model (automatically binds it)

    local rockEntity = scene:create_entity("rockEntity", "")
    local rockModel = rockEntity:add_model("coreAssets/models/Imported/OpenGameArt/mastahcez/stone.fbx", 75.0, false, true) -- get model and add a drawable component
    rockEntity:set_shader("coreAssets/shaders/instance.prog") -- set the shader on the model (automatically binds it)
    
    local treeShader = tree:get_shader(); -- get the shader from the model
    local rockShader = rockModel:get_shader(); -- get the shader from the model

    -- Trees
    for i = 1, TREEAMOUNT do
        local mdl_m = Matrix4x4.new(1.0);

        local scl = Vector3.new(1.0);
        local pos = Vector3.new(0.0);

        while(pos.y < WATERLEVEL)
        do
            pos = Vector3.new(math.random(-512.0, 512.0), 0.0, math.random(-512.0, 512.0));

            local terr_scale = scene.globals.terrain.transform.scale;
            pos.y = (scene.globals.terrain.terr:get_height(pos.x / terr_scale.x, pos.z / terr_scale.z) * terr_scale.y) + 0.015;
        end

        mdl_m = translate(mdl_m, pos);

        mdl_m = rotate(mdl_m, math.rad(0.0), Vector3.new(1.0, 0.0, 0.0));
        mdl_m = rotate(mdl_m, math.rad(math.random(-90.0, 90.0)), Vector3.new(0.0, 1.0, 0.0));
        mdl_m = rotate(mdl_m, math.rad(0.0), Vector3.new(0.0, 0.0, 1.0));

        mdl_m = scale(mdl_m, scl);

        treeMatrices[i] = mdl_m;
    end

    instantiate_model(tree, treeMatrices, TREEAMOUNT);

    treeShader:set_int("diffuse_map", 0);

    -- Rocks
    for i = 1, ROCKAMOUNT do
        local mdl_m = Matrix4x4.new(1.0);

        local scl = Vector3.new(1.0);
        local pos = Vector3.new(0.0);

        while(pos.y < WATERLEVEL)
        do
            pos = Vector3.new(math.random(-512.0, 512.0), 0.0, math.random(-512.0, 512.0));

            local terr_scale = scene.globals.terrain.transform.scale;
            pos.y = (scene.globals.terrain.terr:get_height(pos.x / terr_scale.x, pos.z / terr_scale.z) * terr_scale.y) + 0.25;
        end

        mdl_m = translate(mdl_m, pos);

        mdl_m = rotate(mdl_m, math.rad(0.0), Vector3.new(1.0, 0.0, 0.0));
        mdl_m = rotate(mdl_m, math.rad(math.random(-90.0, 90.0)), Vector3.new(0.0, 1.0, 0.0));
        mdl_m = rotate(mdl_m, math.rad(0.0), Vector3.new(0.0, 0.0, 1.0));

        mdl_m = scale(mdl_m, scl);

        rockMatrices[i] = mdl_m;
    end

    instantiate_model(rockModel, rockMatrices, ROCKAMOUNT);

    rockShader:set_int("diffuse_map", 0);
end

function update()

end