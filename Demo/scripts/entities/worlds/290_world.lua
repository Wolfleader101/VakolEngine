function init()
    local levels = {}; 
    local props = {};
    
    levels[1] = scene:create_entity("Opaque Level", "");
    levels[2] = scene:create_entity("Transparent Level", "");

    props[1] = scene:create_entity("Opaque Props", "");
    props[2] = scene:create_entity("Transparent Props", "");

    levels[1]:add_shaded_model("assets/models/290_level/level_opaque.fbx", 1.0, "assets/shaders/toon.prog");
    levels[2]:add_shaded_model("assets/models/290_level/level_transparent.fbx", 1.0, "assets/shaders/toon.prog");

    props[1]:add_shaded_model("assets/models/290_level/level_opaque_props.fbx", 1.0, "assets/shaders/toon.prog");
    props[2]:add_shaded_model("assets/models/290_level/level_transparent_props.fbx", 1.0, "assets/shaders/toon.prog");

    levels[1]:get_transform().pos = Vector3.new(470.0, -340.0, -1487.4);
    levels[2]:get_transform().pos = Vector3.new(470.0, -340.0, -1487.4);

    props[1]:get_transform().pos = Vector3.new(470.0, -340.0, -1487.4);
    props[2]:get_transform().pos = Vector3.new(470.0, -340.0, -1487.4);
end

function update()
end