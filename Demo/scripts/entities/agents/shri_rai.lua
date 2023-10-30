local emotions;
local piss;
local nav;

local target;

local rb;
local trans;

local states <const> = { "flee", "chase", "wander", "idle" };

local piss_locations = { Vector3.new(17, 2.2, 137), Vector3.new(1.4, 1.5, 36.5), Vector3.new(32, 2.5, 20) };
local piss_lookat = { Vector3.new(-6, 4.5, 148), Vector3.new(7.7, 2.7, 41.721), Vector3.new(-32, 2.5, 45) };
local piss_index = 3;

local ANGER_RAD <const> = 10;
local ANGER_THRESHOLD <const> = 0.5;


function init()
    entity:add_model("assets/models/ai/shri/shri.fbx", 1);

    trans = entity:get_transform();

    entity:add_script("emotions", "components/emotion.lua");
    emotions = entity:get_script("emotions");

    entity:add_script("piss", "components/piss.lua");
    piss = entity:get_script("piss");

    rb = entity:add_rigid();
    rb.type = BodyType.Dynamic;
    rb.rot_lock = BVector3.new(true, true, true);
    entity:add_box_collider(Vector3.new(0.9, 1.75, 0.3));

    entity:add_script("navigation", "components/navigation.lua");
    nav = entity:get_script("navigation");

    nav.MOVE_SPEED = 0.025;
    nav.ROTATE_SPEED = 2.5;
    nav.BRAKE_FORCE = 1.0;



    --nav.TARGET = Vector3.new(0, 0, 0);
    nav.TARGET = piss_locations[piss_index];

    --print(nav.TARGET.x .. " " .. nav.TARGET.y .. " " .. nav.TARGET.z)
    nav.MAX_DISTANCE = 0.1;
    nav.set_state("chase");
    nav.look_at(nav.TARGET, true);


    --nav.set_state(states[math.random(4)]);
end

local pos;
local pissTimer <const> = 7 * 60; -- 7 seconds
local pissTicks = 0;

local attackCooldownTime <const> = 15 * 60;
local attackCooldown = -1;

local foundAngryEntity = false;

function tick()
    pos = trans.pos;

    local currentState = nav.get_state();

    if (currentState == "idle") then
        rb.linearVelocity = Vector3.new();
        pissTicks = pissTicks + 1;
        if (pissTicks < pissTimer) then
            nav.smooth_look_at(piss_lookat[piss_index], false);
            shoot_piss();
        else
            -- Find a new piss spot
            piss_index = math.random(#piss_locations);
            nav.TARGET = piss_locations[piss_index];
            nav.set_state("wander");
            pissTicks = 0;
            piss.clean();

            print("new piss target: " .. piss_index .. "\nCleaned up piss");
        end
    else
        if (not foundAngryEntity and attackCooldown < 0) then
            local emotionalEntities = scene.globals.emotional_entities;

            for i = 1, #emotionalEntities do
                if (not (emotionalEntities[i] == entity)) then
                    local entityPos = emotionalEntities[i]:get_transform().pos;
                    local distanceToEntity = (entityPos - pos):length();
                    if (distanceToEntity < ANGER_RAD) then
                        local entityEmotion = emotionalEntities[i]:get_script("emotions");
                        if entityEmotion.get_emotion(emotions.ANGER) > ANGER_THRESHOLD then
                            foundAngryEntity = true;
                            target = entityPos;
                            print("target spotted: " .. emotionalEntities[i]:get_tag());
                            break;
                        end
                    end
                end
            end

            if foundAngryEntity then
                emotions.set_emotion(emotions.ANGER, 0.7); -- shri angry
                emotions.set_emotion(emotions.JOY, 0)
                nav.TARGET = target;
                nav.set_state("chase");
            end
        end
    end
    -- need attacking
    --local angerLevel = emotions.get_emotion(emotions.ANGER);

    --print()
    if foundAngryEntity and (nav.TARGET - pos):length() < 3 then
        print("Shri kicks the entity!");
        nav.set_state("idle");
        emotions.set_emotion(emotions.JOY, 0.5);
        emotions.set_emotion(emotions.ANGER, -0.1);
        foundAngryEntity = false
        attackCooldown = attackCooldownTime;
    end
    attackCooldown = attackCooldown - 1;
end

function shoot_piss()
    local trans = entity:get_transform();
    local pos = Vector3.new();

    pos.x = trans.pos.x;
    pos.y = trans.pos.y - 0.8;
    pos.z = trans.pos.z;

    local forward = Vector3.new();

    forward.x = trans.forward.x;
    forward.y = trans.forward.y;
    forward.z = trans.forward.z;

    piss.piss(pos + (forward * 0.3), forward);
end
