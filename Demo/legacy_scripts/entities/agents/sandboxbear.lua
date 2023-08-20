local ANIMATIONS = {
        ATTACK = 0,
        DIE = 1,
        EAT = 2,
        IDLE = 3,
        RUN = 4,
        WALK = 6
}

local enemyAttackAnimDistance = 1.2;
local giveUpDistance = 8.0;
local speed = 0.4;
local sprint_speed = 1.6;
local dir = Vector3.new(math.random() * 2 - 1, 0, math.random() * 2 - 1);
    
local WAIT_TIMER = 0.0;
local DIR_TIMER = 0.0;
local VIEW_DISTANCE = 10.0;
local SPOTTED = false;

local fsm = nil;

    
function setup_fsm()
    fsm = entity:add_fsm();

    fsm:add_state("eating", function()
        --entity:play_animation(ANIMATIONS.EAT);
        -- entity:get_rigid():set_velocity(Vector3.new(0,0,0));
        if(fsm_wait(math.random(5,7))) then
            fsm:change_state("roaming")
        end
    end)

    fsm:add_state("attack", function()
        trigger_nearby_bears(entity, 12.0);
        -- entity:get_rigid():set_velocity(Vector3.new(0,0,0));
    
        --entity:play_animation(ANIMATIONS.ATTACK);

        if(fsm_wait(0.75)) then
            if player_distance() > enemyAttackAnimDistance then
                fsm:change_state("running_towards");
            end
        end
    end)

    fsm:add_state("idle", function()
        --entity:play_animation(ANIMATIONS.IDLE);
        -- entity:get_rigid():set_velocity(Vector3.new(0,0,0));

        if (fsm_wait(math.random(5, 7))) then
            local rand = math.random();
            if (rand < 0.6) then
                fsm:change_state("roaming")
            else
                fsm:change_state("eating")
            end
        end
    end)

    fsm:add_state("roaming", function()
        local stateChange = false
        if (fsm_wait(math.random(5, 7))) then
            local rand = math.random();
            if (rand < 0.4) then
                fsm:change_state("idle")
                stateChange = true
            elseif (rand < 0.8) then
                fsm:change_state("eating")
                stateChange = true
            end
        end

        if(SPOTTED) then
            SPOTTED = false;
        end
        
        --if not stateChange then
        --    entity:play_animation(ANIMATIONS.WALK);
        --end

        if (dir_wait(math.random(4,6))) then
            dir.x = math.random() * 2 - 1
            dir.z = math.random() * 2 - 1
            while dir:magnitude() == 0 do
                dir.x = math.random() * 2 - 1
                dir.z = math.random() * 2 - 1
            end
            dir:normalize()
        end
        
        local velocity = speed * Time.delta_time;
        local move = dir * velocity;
        -- entity:get_rigid():set_velocity(move);
        entity:get_transform().pos = entity:get_transform().pos + move;

        local targetRotation = math.atan(dir.x, dir.z)
        targetRotation = targetRotation * (180 / math.pi)
        entity:get_transform().rot.y = targetRotation

        if player_distance() < enemyAttackAnimDistance then
            fsm:change_state("attack");
        end
    end)

    fsm:add_state("running_towards", function()
        --entity:play_animation(ANIMATIONS.RUN);

        local diff = scene.globals.player.pos - entity:get_transform().pos;
        dir = diff:normalize();
      
        local velocity = sprint_speed * Time.delta_time;
        local move = dir * velocity;

        -- entity:get_rigid():set_velocity(move);
        entity:get_transform().pos = entity:get_transform().pos + move;

        local targetRotation = math.atan(dir.x, dir.z)
        targetRotation = targetRotation * (180 / math.pi)
        entity:get_transform().rot.y = targetRotation

        if player_distance() < enemyAttackAnimDistance then
            fsm:change_state("attack");
        elseif player_distance() > giveUpDistance then
            fsm:change_state("roaming");
        end
    end)

    fsm:add_state("alerted", function()
        --entity:play_animation(ANIMATIONS.RUN);

        local diff = scene.globals.player.pos - entity:get_transform().pos;
        dir = diff:normalize();

        local velocity = sprint_speed * Time.delta_time;
        local move = dir * velocity;

        -- entity:get_rigid():set_velocity(move);
        entity:get_transform().pos = entity:get_transform().pos + move;

        local targetRotation = math.atan(dir.x, dir.z)
        targetRotation = targetRotation * (180 / math.pi)
        entity:get_transform().rot.y = targetRotation

        if player_distance() < enemyAttackAnimDistance then
            fsm:change_state("attack");
        end
    end)

    fsm:change_state("roaming");
end

function init()
    while dir:magnitude() == 0 do
        dir.x = math.random() * 2 - 1
        dir.z = math.random() * 2 - 1
    end

    dir:normalize();

    entity:get_transform().pos = Vector3.new(10, 0, 10);
    entity:get_transform().rot = Vector3.new(-90, 0, 0);

    entity:add_model("assets/models/agents/bear.fbx", 50.0);

   setup_fsm();
end

function trigger_nearby_bears(origin_bear, trigger_distance)
    local origin_pos = origin_bear:get_transform().pos

    for i, bear in ipairs(scene.globals.bears) do
        if bear ~= origin_bear and bear:get_fsm():get_state() ~= "attack" then
            local diff = origin_pos - bear:get_transform().pos
            if diff:magnitude() <= trigger_distance then
                bear:get_fsm():change_state("alerted")
            end
        end
    end
end

function player_distance()
    return (scene.globals.player.pos - entity:get_transform().pos):magnitude();
end

function dir_wait(seconds)
    DIR_TIMER = DIR_TIMER + Time.delta_time;

    if (DIR_TIMER >= seconds) then
        DIR_TIMER = 0
        return true;
    end

    return false;
end

function fsm_wait(seconds)
    WAIT_TIMER = WAIT_TIMER + Time.delta_time;

    if (WAIT_TIMER  >= seconds) then
        WAIT_TIMER = 0
        return true;
    end

    return false;
end

function tick()
    PLAYER = scene.globals.player;

    local pos = entity:get_transform().pos;
    local diff = scene.globals.player.pos - pos;
    local player_dist = diff:magnitude();

    if (player_dist > 50) then
        entity:active_model(false);
        return;
    else
        entity:active_model(true);
    end

    if (player_dist < VIEW_DISTANCE) and (player_dist >= enemyAttackAnimDistance) then
        local diff_normal = diff:normalize();
        local dot = diff_normal:dot(dir)

        if (dot > 0) then
            fsm:change_state("running_towards");
        end
    end
    fsm:update()

    --local terr_scale = scene.globals.terrain.transform.scale;
    --pos.y = (scene.globals.terrain.terr:get_height(pos.x / terr_scale.x, pos.z / terr_scale.z) * terr_scale.y) + 0.03;
    pos.y = 0.0;

end