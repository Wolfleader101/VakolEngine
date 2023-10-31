function init()
    local levels = {}; 
    local props = {};
    
    levels[1] = scene:create_entity("Opaque Level", "");
    levels[2] = scene:create_entity("Transparent Level", "");

    props[1] = scene:create_entity("Opaque Props", "");
    props[2] = scene:create_entity("Transparent Props", "");

    local opaque_level = levels[1]:add_shaded_model("assets/models/290_level/level_opaque.fbx", 1.0, "assets/shaders/toon.prog");
    local transparent_level = levels[2]:add_shaded_model("assets/models/290_level/level_transparent.fbx", 1.0, "assets/shaders/toon.prog");

    local opaque_level_props = props[1]:add_shaded_model("assets/models/290_level/level_opaque_props.fbx", 1.0, "assets/shaders/toon.prog");
    local transparent_level_props = props[2]:add_shaded_model("assets/models/290_level/level_transparent_props.fbx", 1.0, "assets/shaders/toon.prog");

    levels[1]:get_transform().pos = Vector3.new(470.0, -340.0, -1487.4);
    levels[2]:get_transform().pos = Vector3.new(470.0, -340.0, -1487.4);

    props[1]:get_transform().pos = Vector3.new(470.0, -340.0, -1487.4);
    props[2]:get_transform().pos = Vector3.new(470.0, -340.0, -1487.4);

    for i = 1, opaque_level:get_mesh_count() do
        opaque_level:get_mesh(i - 1).material:set_light_position(Vector3.new(1.0, 0.0, 0.0));
    end

    for i = 1, opaque_level_props:get_mesh_count() do
        opaque_level_props:get_mesh(i - 1).material:set_light_position(Vector3.new(1.0, 0.0, 0.0));
    end

    for i = 1, transparent_level:get_mesh_count() do
        transparent_level:get_mesh(i - 1).material:set_diffuse_color(Vector4.new(1.0, 1.0, 1.0, 0.0));
    end

    for i = 1, transparent_level_props:get_mesh_count() do
        transparent_level_props:get_mesh(i - 1).material:set_diffuse_color(Vector4.new(1.0, 1.0, 1.0, 0.0));
    end
end

function update()
end