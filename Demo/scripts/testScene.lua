function init(scene)
    scene:create_entity("cameraController.lua");
    scene:create_entity("player.lua");
    local terrain = scene:get_terrain();
    terrain:load_heightmap("coreAssets/textures/Heightmaps/height128.raw")
    terrain:load_texture("coreAssets/textures/Terrain/TEX_128_GRASS.jpg")
    terrain:generate();

    for i= 1,15 do
        local x = math.random(0, terrain:get_size());
        local z = math.random(0, terrain:get_size());
        local y = terrain:get_height(x, z) + 2;
        local ent = scene:create_entity("");
        ent:add_model("coreAssets/models/cube.obj");
        ent:get_transform().pos.x = x;
        ent:get_transform().pos.z = z;
        ent:get_transform().pos.y = y;
    end
end


function update(scene)
    -- print("Hello from TestScene update!!!");
    -- print(Time["delta_time"]);
end