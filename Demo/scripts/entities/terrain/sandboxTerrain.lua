function init()
    print("Initialising Terrain");
  
    local terrain = entity:create_height_map_terrain(scene, "assets/textures/Heightmaps/height128.raw", 0, 0); -- size

    local model = terrain:get_model();

    entity:add_texture(0, "assets/textures/grass.png", false, false);

    entity:set_shader("assets/shaders/basic_terrain.prog");
    local shader = model:get_shader();

    shader:set_vec2v("uv_scale", Vector2.new(100));
    shader:set_int("layer_1", 0);

    scene.globals.terrain = { transform = entity:get_transform(), terr = entity:get_terrain() };

end

function update()
end