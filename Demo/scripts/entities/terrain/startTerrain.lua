function init()
    print("Initialising Terrain");
  
    local terrain = entity:create_height_map_terrain("coreAssets/textures/Heightmaps/height128.raw", 0, 50); -- size

    local model = terrain:get_model();

    model:set_shader("coreAssets/shaders/start_terrain.prog");
    local shader = model:get_shader();

    shader:set_float("u_time", Time.curr_time);
    local trans = entity:get_transform();
    trans.scale.x = 1000;

    scene.globals.terrain = {transform = entity:get_transform(), terr = entity:get_terrain()};
end

function update()
    
    local terrain = entity:get_terrain();
    local model = terrain:get_model();
    local shader = model:get_shader();

    shader:set_float("u_time", Time.curr_time);

end