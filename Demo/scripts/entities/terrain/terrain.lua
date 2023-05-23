function init()
    print("Initialising Terrain");
    local terrain = entity:create_height_map_terrain("coreAssets/textures/Heightmaps/height128.raw", 0, 30);

    local lightmap = create_raw_texture("coreAssets/textures/lightmap.raw");
    
    local layer_1 = create_texture("coreAssets/textures/extras/clover.png", false, false);
    local layer_2 = create_texture("coreAssets/textures/extras/grass.png", false, false);
    local layer_3 = create_texture("coreAssets/textures/extras/grass_dark.png", false, false);
    local layer_4 = create_texture("coreAssets/textures/extras/cliff_rock.jpg", false, false);
    local layer_5 = create_texture("coreAssets/textures/extras/rocks.jpg", false, false);
    local layer_6 = create_texture("coreAssets/textures/extras/snow.png", false, false);

    local model = terrain:get_model();
    local mesh = model:get_mesh(0);
    local material = mesh:get_material();

    material:add_texture(lightmap);

    material:add_texture(layer_1);
    material:add_texture(layer_2);
    material:add_texture(layer_3);
    material:add_texture(layer_4);
    material:add_texture(layer_5);
    material:add_texture(layer_6);

    model:set_shader("coreAssets/shaders/basic_terrain.prog");
    local shader = model:get_shader();

    shader:set_vec3v("light.position",  Vector3.new(0.0, 60.0, 0.0));
    shader:set_vec3v("light.direction", Vector3.new(math.rad(0.0), math.rad(0.0), math.rad(-90.0)))
    shader:set_vec2v("uv_scale", Vector2.new(10));

    shader:set_int("light_map", 0);

    shader:set_int("layer_1", 1);
    shader:set_int("layer_2", 2);
    shader:set_int("layer_3", 3);
    shader:set_int("layer_4", 4);
    shader:set_int("layer_5", 5);
    shader:set_int("layer_6", 6);

    scene.globals.terrain = {transform = entity:get_transform(), terr = entity:get_terrain()};
end

function update()

end