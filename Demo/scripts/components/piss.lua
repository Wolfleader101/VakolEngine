local piss_particles = {}
local piss_index = 1;
local MAX_PISS <const> = 100;

local y_storage;

function init()
    local tag = entity:get_tag();
    y_storage = randomFloat(-1000, -100);
    for i = 1, MAX_PISS do
        piss_particle = scene:create_entity(tag .. " piss " .. i, "");

        local piss_trans = piss_particle:get_transform();
        piss_trans.pos = Vector3.new(i, y_storage, i);
        piss_trans.scale = Vector3.new(0.05, 0.05, 0.05);

        local model = piss_particle:add_model("coreAssets/models/sphere.obj", 1);
        local mesh = model:get_mesh(0);
        -- https://www.colourlovers.com/color/BCB502/piss_yellow
        mesh.material:set_diffuse_color(Vector4.new(0.7373, 0.7098, 0.0078, 1.0));

        local rb = piss_particle:add_rigid();
        piss_particle:add_sphere_collider(0.025);
        piss_particles[i] = piss_particle;
        rb.hasGravity = false;
        rb.is_sleeping = true;
    end
end

local function randomFloat(lower, greater)
    return lower + math.random() * (greater - lower);
end

function piss(pos, forward)
    print("pos: " .. pos.x .. " " .. pos.y .. " " .. pos.z);
    print("forward: " .. forward.x .. " " .. forward.y .. " " .. forward.z);

    piss_index = (piss_index % MAX_PISS) + 1;
    local particle = piss_particles[piss_index];
    particle:get_transform().pos = pos;

    local rb = particle:get_rigid();
    --rb.linearVelocity = Vector3.new(0, 0, 0);
    rb.hasGravity = true;
    rb.is_sleeping = false;

    local randVec = Vector3.new(randomFloat(0, 1), randomFloat(0, 1), randomFloat(0, 1));
    rb.linearVelocity = forward + randVec;
end

function clean()
    for i = 1, MAX_PISS do
        local trans = piss_particles[i]:get_transform();
        trans.pos = Vector3.new(i, y_storage, i);

        local rb = piss_particles[i]:get_rigid();
        rb.hasGravity = false;
        rb.is_sleeping = true;
    end
end
