function init()
    print("Initialising Terrain");
  
    local terrain = entity:create_height_map_terrain("coreAssets/textures/Heightmaps/height128.raw", -10, 50); -- size

    local model = terrain:get_model();

    entity:set_shader("coreAssets/shaders/start_terrain.prog");
    local shader = model:get_shader();

    shader:set_float("u_time", Time.curr_time);
    local trans = entity:get_transform();
    trans.scale = Vector3.new(3, 5, 10);

    trans.pos.z = -1000;

    scene.globals.terrain = {transform = entity:get_transform(), terr = entity:get_terrain()};
    toggle_skybox();
    --toggle_wireframe();
    
end

function update()
    toggle_wireframe();
    clear_color(0.00961, 0.0431, 0.0784, 0);
    
    local terrain = entity:get_terrain();
    local model = terrain:get_model();
    local shader = model:get_shader();

    shader:set_float("u_time", Time.curr_time * 10);
    toggle_wireframe();
    
end