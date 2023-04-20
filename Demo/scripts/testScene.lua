function init(scene)
    scene:create_entity("cameraController.lua");
    scene:create_entity("advanced_example.lua"); -- switch between basic or advanced_example
    scene:create_entity("water_example.lua");
end


function update(scene)
    -- print("Hello from TestScene update!!!");
    -- print(Time["delta_time"]);
end