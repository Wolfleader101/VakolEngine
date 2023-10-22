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

local WANDER_TICKS = 300; -- 5 seconds
local wanderCount = 0;
local wanderChanges = 0; -- used to switch to wait state

local WAIT_TICKS = 600; -- 10 seconds 
local waitCount = 0;

local DIR_CHANGE_COOLDOWN = 300; -- 5 second
local dirChangeCounter = 0;

local currentRotation = 0;
local targetRotation = 0;
local ROTATION_LERP_SPEED = 0.05; -- lerping bruvva

function tick()

    if state == "navigate" then

        trans = entity:get_transform();
        dir = get_direction();
        targetRotation = math.atan(dir.x, dir.z) * (180 / math.pi);
        entity:get_transform().rot.y = targetRotation;

        dist = distance();

        if dist < MAX_DIST then
            state = "wander";
            rb = entity:get_rigid();
            rb.linearVelocity = Vector3.new(0, 0, 0);
        else

            info = RayCastHitInfo.new();

            local rayOrigin = trans.pos + Vector3.new(0, 5, 0); -- so we dont cast from inside agent collider

            if scene:raycast(rayOrigin, dir, 0.25, info) and info.distance < CHECK_DIST then
                -- get normal and change dir to follow it. Not the best but will do the job
                local reflectDir = dir - 2 * (dir:dot(info.normal)) * info.normal;
                dir = reflectDir:normalize()
            end
        end

    elseif state == "wait" then

        if waitCount >= WAIT_TICKS then
            state = "wander"; -- Transition back to wander after waiting
            waitCount = 0;
        else
            waitCount = waitCount + 1;
        end

    else -- wander state

        if dirChangeCounter > 0 then
            dirChangeCounter = dirChangeCounter - 1;
        end

        if wanderCount == 0 or wanderCount >= WANDER_TICKS then
            if dirChangeCounter == 0 then
                local randomAngle = math.random() * 2 * math.pi;
                local newDir = Vector3.new(math.cos(randomAngle), 0, math.sin(randomAngle));
                dir = dir * 0.8 + newDir * 0.2; -- smooth turn w Interpolate
                wanderCount = 0;
                dirChangeCounter = DIR_CHANGE_COOLDOWN;
                wanderChanges = wanderChanges + 1;
            end
        else
            wanderCount = wanderCount + 1;
        end

        targetRotation = math.atan(dir.x, dir.z) * (180 / math.pi);
        entity:get_transform().rot.y = targetRotation;

        info = RayCastHitInfo.new();

        -- slide along obstacle
        if scene:raycast(trans.pos, dir, CHECK_DIST, info) and info.hit and dirChangeCounter == 0 then
            slideDir = Vector3.new(-info.normal.z, 0, info.normal.x);
            dir = dir * 0.8 + slideDir * 0.2; -- Interpolate
            dirChangeCounter = DIR_CHANGE_COOLDOWN;
        end

        targetRotation = math.atan(dir.x, dir.z) * (180 / math.pi);

        if wanderChanges == 3 then
            state = "wait";
            wanderChanges = 0;
            rb = entity:get_rigid();
            rb.linearVelocity = Vector3.new(0, 0, 0);
        end

    end

    currentRotation = currentRotation + (targetRotation - currentRotation) * ROTATION_LERP_SPEED;
    entity:get_transform().rot.y = currentRotation;

    rb = entity:get_rigid();
    vel = Vector2.new(rb.linearVelocity.x, rb.linearVelocity.z):magnitude();

    if vel < MAX_SPEED and state ~= "wait" then
        rb:apply_impulse(dir);
    end
end
