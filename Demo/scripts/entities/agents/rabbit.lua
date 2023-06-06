function init()
    state.ANIMATIONS = {
        DIE = 0,
        EAT = 1,
        IDLE = 2,
        RUN = 3,
        STATIC = 4,
        WALK = 5,
    }

    state.speed = 0.5;
    state.sprint_speed = 3;
    state.dir = Vector3.new(0);
    state.dir.x = math.random(-1, 1);
    state.dir.z = math.random(-1, 1);

    state.WAIT_TIMER = 0.0;
    state.DIR_TIMER = 0.0;

    state.VIEW_DISTANCE = 7.5;
    state.SPOTTED = false;

    entity:get_transform().pos = Vector3.new(10, 0, 10);

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

    print("here");
    state.fsm = entity:add_fsm();
    print("yeet");
    state.fsm:add_state("eating", function()
        entity:set_animation_state(state.ANIMATIONS.EAT);
        if(fsm_wait(math.random(5,7))) then
            state.fsm:change_state("roaming")
        end

    end)

    state.fsm:add_state("looking", function()
        entity:set_animation_state(state.ANIMATIONS.IDLE);

        if(fsm_wait(math.random(5,7))) then
            local rand = math.random();
            if (rand < 0.6) then
                state.fsm:change_state("roaming")
            else
                state.fsm:change_state("eating")
            end
        end

    end)

    state.fsm:add_state("roaming", function()
        if (fsm_wait(math.random(5, 7))) then
            local rand = math.random();
            if (rand < 0.4) then
                state.fsm:change_state("looking")
            elseif (rand < 0.8) then
                state.fsm:change_state("eating")
            end
        end

        if(state.SPOTTED) then
            state.SPOTTED = false;
        end
        
        entity:set_animation_state(state.ANIMATIONS.WALK);


        if (dir_wait(math.random(4,6))) then
            state.dir.x = math.random(-1, 1);
            state.dir.z = math.random(-1, 1);
        end
        
        local velocity = state.speed * Time.delta_time;

        local pos = entity:get_transform().pos;
        pos.x = pos.x + (state.dir.x * velocity);
        pos.z = pos.z + (state.dir.z * velocity);
    end)

    state.fsm:add_state("running_away", function()
        print("RUNNING")
        entity:set_animation_state(state.ANIMATIONS.RUN);

        local diff = scene.globals.player.pos - entity:get_transform().pos;
        local player_dist = diff:magnitude();

        if(not state.SPOTTED or dir_wait(4)) then
            local rand_dir = Vector3.new(math.random() - 0.5, 0, math.random() - 0.5)
            rand_dir:normalize()

            -- Check if the random direction is pointing towards the player
            local dotProduct = rand_dir:dot(diff:normalize())
                while dotProduct > 0.5 do
                    rand_dir = Vector3.new(math.random() - 0.5, 0, math.random() - 0.5)
                    rand_dir:normalize()
                    dotProduct = rand_dir:dot(diff:normalize())
                end
            
            -- Update the state direction
            state.dir = rand_dir
            state.SPOTTED = true;
        end

        local velocity = state.sprint_speed * Time.delta_time;

        local pos = entity:get_transform().pos;
        pos.x = pos.x + (state.dir.x * velocity);
        pos.z = pos.z + (state.dir.z * velocity);

        if (player_dist >= state.VIEW_DISTANCE + 1.5 and fsm_wait(math.random(2,4))) then
            state.fsm:change_state("roaming")
        end
    end)

    -- Set the initial state
    state.fsm:change_state("roaming")

    print_err("Rabbity is ready")
end

function dir_wait(seconds)
    state.DIR_TIMER = state.DIR_TIMER + Time.delta_time;

    if (state.DIR_TIMER >= seconds) then
        state.DIR_TIMER = 0
        return true;
    end

    return false;
end

function fsm_wait(seconds)
    state.WAIT_TIMER = state.WAIT_TIMER + Time.delta_time;

    if (state.WAIT_TIMER  >= seconds) then
        state.WAIT_TIMER = 0
        return true;
    end

    return false;
end

function update()
    state.fsm:update()
    local pos = entity:get_transform().pos;
    local terr_scale = scene.globals.terrain.transform.scale;
    pos.y = (scene.globals.terrain.terr:get_height(pos.x / terr_scale.x, pos.z / terr_scale.z) * terr_scale.y) + 0.03;

    local targetRotation = math.atan(state.dir.x, state.dir.z)
    targetRotation = targetRotation * (180 / math.pi)
    entity:get_transform().rot.y = targetRotation

    local diff = scene.globals.player.pos - pos;
    local player_dist = diff:magnitude();

    if (player_dist < state.VIEW_DISTANCE) then
        local diff_normal = diff:normalize();
        local dot = diff_normal:dot(state.dir)

        if (dot > 0) then
            state.fsm:change_state("running_away");
        end
    end
end