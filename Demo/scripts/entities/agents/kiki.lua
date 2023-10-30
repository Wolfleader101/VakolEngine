local kikis = {};
local MAX_KIKIS <const> = 4;

function random_float(lower, greater)
    return lower + math.random() * (greater - lower);
end

function create_kikis()
    local models = {};

    for i = 1, MAX_KIKIS do
        kikis[i] = scene:create_entity("Kiki " .. i, "");
        kikis[i]:get_transform().pos = Vector3.new(i * 5.0, 5.0, 0.0);

        models[i] = kikis[i]:add_model("assets/models/ai/kiki/kiki.obj", 0.5);
        
        local body_mesh = models[i]:get_mesh(0);
        local eye_mesh = models[i]:get_mesh(1);

        body_mesh.material:set_diffuse_color(Vector4.new(random_float(0.0, 1.0), random_float(0.0, 1.0), random_float(0.0, 1.0), 1.0));
        body_mesh.material:use_colors_and_textures(true);
        
        body_mesh.material:set_light_type(LightType.Directional);
        body_mesh.material:set_light_direction(Vector3.new(0.0, 1.0, 0.0));

        eye_mesh.material:set_opacity(0.0);
        eye_mesh.material:use_lighting(false);

        local rb = kikis[i]:add_rigid();
        rb.rot_lock = BVector3.new(true, true, true);

        kikis[i]:add_box_collider(models[i]:get_half_extents(0));

        kikis[i]:add_script("navigation", "components/navigation.lua");
        local nav = kikis[i]:get_script("navigation");

        -- OFFSETS FOR KIKI EYE EXPRESSIONS
        -- (0.0, 0.0), (0.0, 1.0), (1.0, 0.0), (1.0, 1.0) = neutral
        -- (0.0, 0.5), (0.0, 1.5), (1.0, 0.5), (1.0, 1.5) = happy
        -- (0.5, 0.0), (0.5, 1.0), (1.5, 0.0), (1.5, 1.0) = angry
        -- (0.5, 0.5), (0.5, 1.5), (1.5, 0.5), (1.5, 1.5) = dizzy

        eye_mesh.material:set_uv_offset(Vector2.new(0.5, 0.0));

        nav.TARGET = scene:get_camera():get_pos();
 
        nav.MOVE_SPEED = 0.05;
        nav.ROTATE_SPEED = 5.0;
        nav.BRAKE_FORCE = 1.0;

        nav.set_state("chase");
    end
end

function init()
    create_kikis();
end