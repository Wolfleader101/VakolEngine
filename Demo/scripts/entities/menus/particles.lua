local particles = {}
local index = 1;
local MAX_PARTICLE <const> = 300;

local y_storage;

function init()
    local tag = entity:get_tag();
    y_storage = randomFloat(-1000, -100);
    for i = 1, MAX_PARTICLE do
        particle = scene:create_entity(" particle " .. i, "");

        local particle_trans = particle:get_transform();
        particle_trans.pos = Vector3.new(i, y_storage, i);
        -- particle_trans.scale = Vector3.new(0.05, 0.05, 0.05);
        particle_trans.scale = Vector3.new(0.3, 0.3, 0.3);

        local model = particle:add_model("coreAssets/models/sphere.obj", 1);
        local mesh = model:get_mesh(0);

        -- mesh.material:set_diffuse_color(Vector4.new(randomFloat(0.3, 0.5), randomFloat(0.3, 0.5), randomFloat(0.4, 0.7),
        --     1));
        mesh.material:set_diffuse_color(Vector4.new(randomFloat(0, 1), randomFloat(0, 1), randomFloat(0, 1), 1));
        --mesh.material:set_diffuse_color(Vector4.new(i / MAX_PARTICLE, MAX_PARTICLE / i, i / MAX_PARTICLE, 1));

        local rb = particle:add_rigid();
        particle:add_sphere_collider(0.025);
        particles[i] = particle;
        rb.hasGravity = false;
        rb.is_sleeping = true;
    end
end

function randomFloat(lower, greater)
    return lower + math.random() * (greater - lower);
end

function shoot_particle(pos, forward)
    index = (index % MAX_PARTICLE) + 1;
    particle = particles[index];
    particle:get_transform().pos = pos;

    rb = particle:get_rigid();

    randVec = Vector3.new(randomFloat(0, 4), randomFloat(0, 4), randomFloat(0, 4));
    --rb:apply_impulse(forward);
    rb:apply_impulse(forward + randVec);
end

function clean()
    for i = 1, MAX_PARTICLE do
        trans = particles[i]:get_transform();
        trans.pos = Vector3.new(i, y_storage, i);

        rb = particles[i]:get_rigid();
        rb.hasGravity = false;
        rb.is_sleeping = false;
    end
end

pos = Vector3.new(-2, -3, -5);
local forward = Vector3.new(-0.5, 0.3, 0.5);

function tick()
    shoot_particle(pos, forward);
end
