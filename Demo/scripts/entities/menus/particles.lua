local particles = {}
local rigids = {}
local transforms = {}

local index = 1;
local MAX_PARTICLE <const> = 300;

local y_storage;

function init()
    local tag = entity:get_tag();
    y_storage = randomFloat(-1000, -100);
    for i = 1, MAX_PARTICLE do
        particle = scene:create_entity(" particle " .. i, "");

        transforms[i] = particle:get_transform();
        transforms[i].pos = Vector3.new(i, y_storage, i);
        -- particle_trans.scale = Vector3.new(0.05, 0.05, 0.05);
        transforms[i].scale = Vector3.new(0.3, 0.3, 0.3);

        local model = particle:add_model("coreAssets/models/sphere.obj", 1);
        local mesh = model:get_mesh(0);

        -- mesh.material:set_diffuse_color(Vector4.new(randomFloat(0.3, 0.5), randomFloat(0.3, 0.5), randomFloat(0.4, 0.7),
        --     1));
        mesh.material:set_diffuse_color(Vector4.new(randomFloat(0, 1), randomFloat(0, 1), randomFloat(0, 1), 1));
        --mesh.material:set_diffuse_color(Vector4.new(i / MAX_PARTICLE, MAX_PARTICLE / i, i / MAX_PARTICLE, 1));

        rigids[i] = particle:add_rigid();
        particle:add_sphere_collider(0.15);
        particles[i] = particle;
        rigids[i].hasGravity = false;
        rigids[i].is_sleeping = true;
    end
end

function randomFloat(lower, greater)
    return lower + math.random() * (greater - lower);
end

function shoot_particle(pos, forward)
    index = (index % MAX_PARTICLE) + 1;
    transforms[index].pos = pos;

    randVec = Vector3.new(randomFloat(0, 4), randomFloat(0, 4), randomFloat(0, 4));
    rigids[index]:apply_impulse(forward + randVec);
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
