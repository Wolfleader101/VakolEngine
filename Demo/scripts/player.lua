function init(entity)
    print_err(entity:get_transform().pos.y);
    entity:get_transform().pos.y = 0.2;
    entity:add_model("cube.obj")
    print("Hello from player");
end


function update(entity)
    print(entity:get_transform().pos.y);
    -- print("Hello from TestScene update");
end