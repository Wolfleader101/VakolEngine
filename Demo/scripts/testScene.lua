function init(scene)
    print("Hello from TestScene");
    scene:create_entity("cameraController.lua");
    entity = scene:create_entity("player.lua");
    transform = entity:get_transform();
    print_err(transform.pos.y);

end


function update(scene)
    -- print("Hello from TestScene update!!!");
    -- print(Time["delta_time"]);
end