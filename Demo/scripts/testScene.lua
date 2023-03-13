function init(scene)
    print("Hello from TestScene");
    scene:create_entity("player.lua");
end


function update()
    -- print("Hello from TestScene update!!!");
    print(Time["delta_time"]);
end