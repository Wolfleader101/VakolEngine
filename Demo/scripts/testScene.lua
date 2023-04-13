function init(scene)
    scene:create_entity("cameraController.lua");
    entity = scene:create_entity("player.lua");
    transform = entity:get_transform();
end


function update(scene)
    -- print("Hello from TestScene update!!!");
    -- print(Time["delta_time"]);
end