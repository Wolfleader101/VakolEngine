function init(scene)
    print("Hello from TestScene");
    entity = scene:create_entity("player.lua");
    transform = entity:get_transform();
    print_err(transform.pos.y);
    entity.add_model("cube.obj")

end


function update()
    -- print("Hello from TestScene update!!!");
    -- print(Time["delta_time"]);
end