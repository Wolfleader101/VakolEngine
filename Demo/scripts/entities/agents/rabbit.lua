function init()
    local IDLE_STATE_DEFAULT = 0;
    local IDLE_STATE_SEARCH = 1;

    local IDLE_STATE_PLAYER_SPOTTED = 2;

    local RUN_STATE_REGULAR = 3;
    local RUN_STATE_ENRAGED = 4;

    local ATTACK_STATE_SLASH_LIGHT = 5;
    local ATTACK_STATE_SLASH_HEAVY = 6;

    local ATTACK_STATE_PUNCH_LIGHT = 7;
    local ATTACK_STATE_SLAM_HEAVY = 8; 

    local MIN_ATTACK_DISTANCE = 0.7;
    local MAX_ATTACK_DISTANCE = 1.2;
    local RUN_DISTANCE = 2.5;
    local VIEW_DISTANCE = 10.0;

    local SPRINT_STATE = 1;

    local RUN_SPEED = 1.25;
    local SPRINT_SPEED = 3.0;

    TIMER = 0.0;
    PAUSE_ANIMATION = false;
    ONCE = false;
    
    entity:get_transform().pos = Vector3.new(2.7, 0, -12.0);

    state.model = entity:add_model("assets/models/agents/rabbit/rabbit.fbx", 1, true, true);
    entity:set_shader("coreAssets/shaders/animation.prog");

    -- state.model:set_animation_state(IDLE_STATE_SEARCH);

    local shader = state.model:get_shader();

    shader:set_vec3v("light.direction", Vector3.new(math.rad(0.0), math.rad(0.0), math.rad(-90.0)));

    shader:set_float("material.shininess", 32.0);
    shader:set_vec3v("tint", Vector3.new(0.2, 0.65, 0.9));

    shader:set_int("material.diffuse_map", 0);
    shader:set_int("material.specular_map", 1);
    shader:set_int("material.normal_map", 2);
    shader:set_int("material.emission_map", 3);

    state.fsm = entity:add_fsm();

    state.fsm:add_state("idle", function()

    end)

    state.fsm:add_state("eating", function()

    end)

    state.fsm:add_state("looking", function()

    end)

    state.fsm:add_state("roaming", function()

    end)

    state.fsm:add_state("running_away", function()
       
    end)



    -- Set the initial state
    state.fsm:change_state("idle")

    print_err("Rabbity is ready")
end

function update()
    state.fsm:update()
    local pos = entity:get_transform().pos;
    local terr_scale = scene.globals.terrain.transform.scale;
    pos.y = (scene.globals.terrain.terr:get_height(pos.x / terr_scale.x, pos.z / terr_scale.z) * terr_scale.y) + 0.05;
end