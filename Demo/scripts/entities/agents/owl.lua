function init()
    state.ANIMATIONS = {
        STATIC = 0,
        FLY = 1,
        STAND = 2,
    }

    state.dir = Vector3.new(math.random() * 2 - 1, 0, math.random() * 2 - 1);
    while state.dir:magnitude() == 0 do
        state.dir.x = math.random() * 2 - 1
        state.dir.z = math.random() * 2 - 1
    end
    state.dir:normalize();

    state.speed = 2;

    state.flyHeight = 10; -- the height of the bird's flying path above the terrain
    state.flyAmplitude = 2; -- the amplitude of the bird's up and down movement
    state.flyTimer = 0;

    state.DIR_TIMER = 0.0;

    state.model = entity:add_model("assets/models/agents/rabbit.fbx", 0.25, true, true);
    entity:set_shader("coreAssets/shaders/animation.prog");

    local shader = state.model:get_shader();

    shader:set_vec3v("light.direction", Vector3.new(math.rad(0.0), math.rad(0.0), math.rad(-90.0)));

    shader:set_float("material.shininess", 32.0);
    shader:set_vec3v("tint", Vector3.new(0.2, 0.65, 0.9));

    shader:set_int("material.diffuse_map", 0);
    shader:set_int("material.specular_map", 1);
    shader:set_int("material.normal_map", 2);
    shader:set_int("material.emission_map", 3);


    entity:set_animation_state(state.ANIMATIONS.FLY);
    print_err("Owl is ready")
end

function dir_wait(seconds)
    state.DIR_TIMER = state.DIR_TIMER + Time.delta_time;

    if (state.DIR_TIMER >= seconds) then
        state.DIR_TIMER = 0
        return true;
    end

    return false;
end


function update()
    local pos = entity:get_transform().pos;
    local terr_scale = scene.globals.terrain.transform.scale;


    if (dir_wait(math.random(15,25))) then
        state.dir.x = math.random() * 2 - 1
        state.dir.z = math.random() * 2 - 1
        while state.dir:magnitude() == 0 do
            state.dir.x = math.random() * 2 - 1
            state.dir.z = math.random() * 2 - 1
        end
        state.dir:normalize()
    end
        
    local velocity = state.speed * Time.delta_time;

    pos.x = pos.x + (state.dir.x * velocity);
    pos.z = pos.z + (state.dir.z * velocity);

    state.flyTimer = state.flyTimer + Time.delta_time;
    local flyOffset = state.flyAmplitude * math.sin(state.flyTimer * 0.5);
    local terrainHeight = scene.globals.terrain.terr:get_height(pos.x / terr_scale.x, pos.z / terr_scale.z) * terr_scale.y;
    pos.y = terrainHeight + state.flyHeight + flyOffset;

    local targetRotation = math.atan(state.dir.x, state.dir.z)
    targetRotation = targetRotation * (180 / math.pi)
    entity:get_transform().rot.y = targetRotation
end