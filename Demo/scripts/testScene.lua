function init(scene)
    scene:create_entity("cameraController.lua");
    -- scene:create_entity("advanced_example.lua"); -- switch between basic or advanced_example

    local terrain = scene:get_terrain();
    terrain:load_heightmap("coreAssets/textures/Heightmaps/height128.raw")
    terrain:generate();
end


function update(scene)
    -- print("Hello from TestScene update!!!");
    -- print(Time["delta_time"]);
end