function init(scene)
    scene:create_entity("cameraController.lua");
    scene:create_entity("spinning_cube.lua");
    scene:create_entity("advanced_example.lua"); -- switch between basic or advanced_example
end


function update(scene)
    -- print("Hello from TestScene update!!!");
    -- print(Time["delta_time"]);
end