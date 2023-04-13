function init(entity)
    print_err(entity:get_transform().pos.y);
    entity:get_transform().pos.y = 0.2;
    entity:add_model("coreAssets/models/cube.obj") --Honestly I think this just does for now while we're on the crunch
    print("Hello from player");
end


function update(entity, scene)
    -- print(entity:get_transform().pos.y);
end