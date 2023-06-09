function init()
    state.ANIMATIONS = {
        CRAWL = 0,
        IDLE = 2,
        RUN = 6,
        ATTACK = 7,
        WALK = 11
    }

    state.enemyAttackAnimDistance = 1.0;
    state.giveUpDistance = 10.0;
    state.speed = 0.3;
    state.sprint_speed = 1.7;
    state.dir = Vector3.new(math.random() * 2 - 1, 0, math.random() * 2 - 1);
    while state.dir:magnitude() == 0 do
        state.dir.x = math.random() * 2 - 1
        state.dir.z = math.random() * 2 - 1
    end
    state.dir:normalize();

    state.WAIT_TIMER = 0.0;
    state.DIR_TIMER = 0.0;

    state.VIEW_DISTANCE = 10.0; -- increased view distance for a deer
    state.SPOTTED = false;

    entity:get_transform().pos = Vector3.new(10, 0, 10);

    state.model = entity:add_model("assets/models/agents/monster.fbx", 0.25, true, true);
    entity:set_shader("coreAssets/shaders/animation.prog");

    local shader = state.model:get_shader();

    shader:set_vec3v("light.direction", Vector3.new(math.rad(0.0), math.rad(0.0), math.rad(-90.0)));

    shader:set_float("material.shininess", 32.0);
    shader:set_vec3v("tint", Vector3.new(0.2, 0.65, 0.9));

    shader:set_int("material.diffuse_map", 0);
    shader:set_int("material.specular_map", 1);
    shader:set_int("material.normal_map", 2);
    shader:set_int("material.emission_map", 3);

    state.fsm = entity:add_fsm();

    state.fsm:add_state("attack", function()
        trigger_nearby_monsters(entity, 10.0);
    
        entity:play_animation(state.ANIMATIONS.ATTACK);

        PLAYER.decrement_health((12.5 * OPTIONS.ATTACK_DAMAGE_DEALT_TO_PLAYER_MULTIPLIER) * Time.delta_time);

        if(fsm_wait(2)) then  -- The attack animation lasts for 2 seconds
            if player_distance() > state.enemyAttackAnimDistance then  -- If player moves more than state.enemyAttackAnimDistance units away, start chasing again
                state.fsm:change_state("running_towards");
            end
        end
    end)

    state.fsm:add_state("idle", function()
        entity:play_animation(state.ANIMATIONS.IDLE);
        if(fsm_wait(math.random(5,7))) then
            local rand = math.random();
            if (rand < 0.6) then
                state.fsm:change_state("roaming")
            end
        end
    end)

    state.fsm:add_state("roaming", function()
        local stateChange = false
        if (fsm_wait(math.random(5, 7))) then
            local rand = math.random();
            if (rand < 0.4) then
                state.fsm:change_state("idle")
                stateChange = true
            end
        end

        if(state.SPOTTED) then
            state.SPOTTED = false;
        end
        
        if not stateChange then
            entity:play_animation(state.ANIMATIONS.WALK);
        end

        if (dir_wait(math.random(4,6))) then
            state.dir.x = math.random() * 2 - 1
            state.dir.z = math.random() * 2 - 1
            while state.dir:magnitude() == 0 do
                state.dir.x = math.random() * 2 - 1
                state.dir.z = math.random() * 2 - 1
            end
            state.dir:normalize()
        end
        
        local velocity = state.speed * Time.delta_time;
        local pos = entity:get_transform().pos;
        pos.x = pos.x + (state.dir.x * velocity);
        pos.z = pos.z + (state.dir.z * velocity);
        entity:get_transform().pos = pos; -- Add this line

        local targetRotation = math.atan(state.dir.x, state.dir.z)
        targetRotation = targetRotation * (180 / math.pi)
        entity:get_transform().rot.y = targetRotation

        -- Added logic to attack the player when close
        if player_distance() < state.enemyAttackAnimDistance then  -- If player is less than 2 units away, start attacking
            state.fsm:change_state("attack");
        end
    end)

    state.fsm:add_state("running_towards", function()
        entity:play_animation(state.ANIMATIONS.RUN);

        -- Now, we want to run towards the player
        local diff = scene.globals.player.pos - entity:get_transform().pos;
        state.dir = diff:normalize();  -- Update direction to run towards player

        local velocity = state.sprint_speed * Time.delta_time;
        local pos = entity:get_transform().pos;
        pos.x = pos.x + (state.dir.x * velocity);
        pos.z = pos.z + (state.dir.z * velocity);
        entity:get_transform().pos = pos; -- Add this line

        local targetRotation = math.atan(state.dir.x, state.dir.z)
        targetRotation = targetRotation * (180 / math.pi)
        entity:get_transform().rot.y = targetRotation

        -- If player is close enough, switch to attack state
        if player_distance() < state.enemyAttackAnimDistance then
            state.fsm:change_state("attack");
        elseif player_distance() > state.giveUpDistance then  -- stop chasing if the player is 20 units away
            state.fsm:change_state("roaming");
        end
    end)

    state.fsm:add_state("alerted", function()
        entity:play_animation(state.ANIMATIONS.RUN);

        -- Now, we want to run towards the player
        local diff = scene.globals.player.pos - entity:get_transform().pos;
        state.dir = diff:normalize();  -- Update direction to run towards player

        local velocity = (state.sprint_speed * OPTIONS.SPRINT_SPEED_MULTIPLIER) * Time.delta_time;
        local pos = entity:get_transform().pos;
        pos.x = pos.x + (state.dir.x * velocity);
        pos.z = pos.z + (state.dir.z * velocity);
        entity:get_transform().pos = pos; -- Add this line

        local targetRotation = math.atan(state.dir.x, state.dir.z)
        targetRotation = targetRotation * (180 / math.pi)
        entity:get_transform().rot.y = targetRotation

        -- If player is close enough, switch to attack state
        if player_distance() < state.enemyAttackAnimDistance then
            state.fsm:change_state("attack");
        end
    end)

    state.fsm:change_state("roaming")

    --print_err("Monster is ready")
end

function trigger_nearby_monsters(origin_monster, trigger_distance)
    local origin_pos = origin_monster:get_transform().pos

    for i, monster in ipairs(scene.globals.monsters) do
        if monster ~= origin_monster and monster:get_fsm():get_state() ~= "attack" then
            local diff = origin_pos - monster:get_transform().pos
            if diff:magnitude() <= trigger_distance then
                monster:get_fsm():change_state("alerted")
                --print("Monster " .. i .. " has been alerted and is now in alerted state.")
            end
        end
    end
end

function player_distance()  -- Added a new function to calculate distance to the player
    return (scene.globals.player.pos - entity:get_transform().pos):magnitude();
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
    PLAYER = scene.globals.player;
    OPTIONS = get_scene("Options Scene").globals.options;

    local pos = entity:get_transform().pos;
    local diff = scene.globals.player.pos - pos;
    local player_dist = diff:magnitude();

    if (player_dist < state.VIEW_DISTANCE) and (player_dist >= state.enemyAttackAnimDistance) then  -- added check here
        local diff_normal = diff:normalize();
        local dot = diff_normal:dot(state.dir)

        if (dot > 0) then
            state.fsm:change_state("running_towards");
        end
    end
    state.fsm:update()

    local terr_scale = scene.globals.terrain.transform.scale;
    pos.y = (scene.globals.terrain.terr:get_height(pos.x / terr_scale.x, pos.z / terr_scale.z) * terr_scale.y) + 0.03;

    entity:get_transform().pos = pos;
end
