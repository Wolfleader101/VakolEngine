local kikis = {};
local MAX_KIKIS <const> = 2;

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
        
        eye_mesh.material:set_opacity(0.0);
        eye_mesh.material:use_lighting(false);
    end
end

function init()
    create_kikis();
end