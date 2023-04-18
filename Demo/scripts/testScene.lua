function init(scene)
    scene:create_entity("cameraController.lua");
    scene:create_entity("player.lua");

    -- local terrain = scene:get_terrain();
    -- terrain:load_heightmap("coreAssets/textures/Heightmaps/height128.raw")
    -- terrain:load_texture("coreAssets/textures/Terrain/TEX_128_GRASS.jpg")
    -- terrain:generate();
end


function update(scene)
    -- print("Hello from TestScene update!!!");
    -- print(Time["delta_time"]);
end