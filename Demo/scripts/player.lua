function init(entity)
    transform = entity:get_transform();
    print_err(transform.pos.y);
    transform.pos.y = 0.2;
    print("Hello from player");
end


function update()
    -- print("Hello from TestScene update");
end