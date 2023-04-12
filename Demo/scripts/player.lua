local ent;
function init(entity)
    ent = entity;
    transform = ent:get_transform();
    print_err(transform.pos.y);
    transform.pos.y = 0.2;
    ent.add_model("cube.obj")
    print("Hello from player");
end


function update()
    -- print("Hello from TestScene update");
end