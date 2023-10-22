DESTINATION = Vector3.new();
local state = "wander"; -- states: "navigate", "wander", "wait"
local trans = Transform.new();
local dir = Vector3.new();

local CHECK_DIST = 1;
local MAX_DIST = 5;
MAX_SPEED = 3;

local function get_direction()
    return (DESTINATION - trans.pos):normalize();
end

local function distance()
    return (DESTINATION - trans.pos):magnitude();
end

function set_state(new_state)
    state = new_state;
end

function init()

end

local WANDER_TICKS = 300; -- 3 seconds
local wanderCount = 0;
local wanderChanges = 0; -- used to switch to wait state
local desiredDir = Vector3.new();

local WAIT_TICKS = 600; -- 10 seconds 
local waitCount = 0;

local DIR_CHANGE_COOLDOWN = 300; -- 5 second
local dirChangeCounter = 0;

local currentRotation = 0;
local targetRotation = 0;
local ROTATION_LERP_SPEED = 0.1; -- lerping bruvva

function tick()
    if state == "navigate" then
        trans = entity:get_transform();
        dir = get_direction();
        targetRotation = math.atan(dir.x, dir.z) * (180 / math.pi);
        dist = distance();

        if dist < MAX_DIST then
            state = "wander";
        else
            info = RayCastHitInfo.new();
            local rayOrigin = trans.pos + Vector3.new(0, 2, 0);

            if scene:raycast(rayOrigin, dir, 0.25, info) and info.distance < CHECK_DIST then
                local reflectDir = dir - 2 * (dir:dot(info.normal)) * info.normal;
                dir = reflectDir:normalize();
            end
        end

    elseif state == "wait" then
        if waitCount >= WAIT_TICKS then
            state = "wander";
            waitCount = 0;
        else
            waitCount = waitCount + 1;
        end

    else -- wander state
        if dirChangeCounter > 0 then
            dirChangeCounter = dirChangeCounter - 1;
        end

        if wanderCount >= WANDER_TICKS then
            if dirChangeCounter == 0 then
                local randomAngle = math.random() * 2 * math.pi;
                local newDir = Vector3.new(math.cos(randomAngle), 0, math.sin(randomAngle));
                dir = dir * 0.8 + newDir * 0.2;
                wanderCount = 0;
                dirChangeCounter = DIR_CHANGE_COOLDOWN;
                wanderChanges = wanderChanges + 1;
            end
        else
            wanderCount = wanderCount + 1;
        end

        info = RayCastHitInfo.new();
        local rayOrigin = trans.pos + dir * 0.5;

        if scene:raycast(rayOrigin, dir, 0.25, info) then
            dir = info.normal;
            local offsetAngle = math.random() * 0.5 - 0.25;
            dir.x = dir.x + math.sin(offsetAngle);
            dir.z = dir.z + math.cos(offsetAngle);
            dirChangeCounter = DIR_CHANGE_COOLDOWN;
        end

        targetRotation = math.atan(dir.z, dir.x) * (180 / math.pi);
        currentRotation = currentRotation + (targetRotation - currentRotation) * ROTATION_LERP_SPEED;

        if wanderChanges == 3 then
            state = "wait";
            wanderChanges = 0;
        end
    end
end

function phys_update()
    entity:get_transform().rot.y = currentRotation;
    rb = entity:get_rigid();
    vel = Vector2.new(rb.linearVelocity.x, rb.linearVelocity.z):magnitude();

    if vel < MAX_SPEED and state ~= "wait" then
        rb:apply_impulse(dir);
    end
end
