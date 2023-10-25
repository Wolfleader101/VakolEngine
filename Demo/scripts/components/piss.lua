local piss_particles = {}
local piss_index = 1;
local MAX_PISS<const> = 100;

local y_storage;

function init()

    tag = entity:get_tag();
    y_storage = randomFloat(-100, -20);
    for i = 1, MAX_PISS do
        piss_particle = scene:create_entity(tag .. " piss " .. i, "");

        piss_trans = piss_particle:get_transform();
        piss_trans.pos = Vector3.new(i, y_storage, i);
        piss_trans.scale = Vector3.new(0.05, 0.05, 0.05);

        model = piss_particle:add_model("coreAssets/models/sphere.obj", 1);
        mesh = model:get_mesh(0);
        -- https://www.colourlovers.com/color/BCB502/piss_yellow
        mesh.material:set_diffuse_color(Vector4.new(0.7373, 0.7098, 0.0078, 1.0));

        rb = piss_particle:add_rigid();
        piss_particle:add_sphere_collider(0.025);
        piss_particles[i] = piss_particle;
        rb.hasGravity = false;
        rb.is_sleeping = true;
    end
end

function randomFloat(lower, greater)
    return lower + math.random() * (greater - lower);
end

function piss(pos, forward)

    piss_index = (piss_index % MAX_PISS) + 1;
    particle = piss_particles[piss_index];
    particle:get_transform().pos = pos;

    rb = particle:get_rigid();
    rb.type = BodyType.Dynamic;

    randVec = Vector3.new(randomFloat(0, 1), randomFloat(0, 1), randomFloat(0, 1));
    rb:apply_impulse((forward * 1.2) + randVec);
end

function clean()

    for i = 1, MAX_PISS do
        trans = piss_particles[i]:get_transform();
        trans = Vector3.new(i, y_storage, i);

        rb = piss_particles[i]:get_rigid();
        rb.hasGravity = false;
        rb.is_sleeping = true;
    end
end
