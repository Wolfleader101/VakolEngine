DESTINATION = Vector3.new();
local state = "wander"; -- states: "navigate", "wander", "wait"
local trans = Transform.new();
local dir = Vector3.new();

local CHECK_DIST = 1;
local MAX_DIST = 5;
MAX_SPEED = 5;

local function get_direction()
    return (DESTINATION - trans.pos):normalize();
end

local function distance()
    return (DESTINATION - trans.pos):magnitude();
end

local function wrapAngle(angle)
    while angle <= -180 do
        angle = angle + 360
    end
    while angle > 180 do
        angle = angle - 360
    end
    return angle
end

function angleDifference(a, b)
    local diff = (b - a + 180) % 360 - 180
    if diff < -180 then
        diff = diff + 360
    end
    return diff
end

local function lerpAngle(a, b, t)
    local delta = angleDifference(a, b)
    local result = a + delta * t
    return wrapAngle(result)
end

local function atan2(y, x)
    if x > 0 then
        return math.atan(y / x)
    elseif y >= 0 and x < 0 then
        return math.atan(y / x) + math.pi
    elseif y < 0 and x < 0 then
        return math.atan(y / x) - math.pi
    elseif y > 0 and x == 0 then
        return math.pi / 2
    elseif y < 0 and x == 0 then
        return -math.pi / 2
    elseif y == 0 and x == 0 then
        return 0
    end
end

function set_state(new_state)
    state = new_state;
end

function init()

end

local WANDER_TICKS = 360; -- 3 seconds
local wanderCount = 0;
local wanderChanges = 0; -- used to switch to wait state

local WAIT_TICKS = 600; -- 10 seconds 
local waitCount = 0;

local DIR_CHANGE_COOLDOWN = 300; -- 5 second
local dirChangeCounter = 0;

local currentRotation = 0;
local targetRotation = 0;

local ROTATION_LERP_SPEED = 0.05; -- lerping bruvva
local ROTATION_THRESHOLD = 5;

function tick()
    trans = entity:get_transform();

    if state == "navigate" then
        dir = get_direction();
        targetRotation = atan2(dir.x, dir.z) * (180 / math.pi);

        -- Lerp the rotation
        currentRotation = lerpAngle(currentRotation, targetRotation, ROTATION_LERP_SPEED);
        entity:get_transform().rot.y = currentRotation;

        print("dir.x:           " .. dir.x);
        print("dir.z:           " .. dir.z);

        print("target:          " .. targetRotation);
        print("currentRotation: " .. currentRotation);

        if distance() < MAX_DIST then
            state = "wander";
        else
            local rayOrigin = trans.pos + Vector3.new(0, 2, 0);
            local info = RayCastHitInfo.new();

            if scene:raycast(rayOrigin, dir, 0.25, info) and info.distance < CHECK_DIST then
                dir = (dir - 2 * (dir:dot(info.normal)) * info.normal):normalize();
            end
        end

    elseif state == "wait" then
        if waitCount < WAIT_TICKS then
            waitCount = waitCount + 1;
        else
            state = "wander";
            waitCount = 0;
        end

    else -- wander state
        if dirChangeCounter > 0 then
            dirChangeCounter = dirChangeCounter - 1;
        end

        if wanderCount >= WANDER_TICKS and dirChangeCounter == 0 then
            local newAngle = math.random() * 2 * math.pi; -- Random angle between 0 and 2π
            dir = Vector3.new(math.cos(newAngle), 0, math.sin(newAngle));

            print(dir.x .. " " .. dir.z);

            wanderCount = 0;
            dirChangeCounter = DIR_CHANGE_COOLDOWN;
            wanderChanges = wanderChanges + 1;

            rb = entity:get_rigid();
            rb.linearVelocity = Vector3.new(0, 0, 0);
        else
            wanderCount = wanderCount + 1;
        end

        -- Rotate to face the wander direction
        targetRotation = math.atan(dir.x, dir.z) * (180 / math.pi);

        -- Lerp the rotation
        local rotationDiff = math.abs(angleDifference(currentRotation, targetRotation));
        if rotationDiff > ROTATION_THRESHOLD then
            -- Lerp the rotation
            currentRotation = lerpAngle(currentRotation, targetRotation, ROTATION_LERP_SPEED);
        end
        entity:get_transform().rot.y = wrapAngle(currentRotation);

        if wanderChanges == 3 then
            state = "wait";
            wanderChanges = 0;
        end

        -- local info = RayCastHitInfo.new();
        -- local rayOrigin = trans.pos + dir * 0.5;

        -- if scene:raycast(rayOrigin, dir, 0.25, info) then
        --     local offsetAngle = math.random() * 0.5 - 0.25;
        --     dir = Vector3.new(info.normal.x + math.sin(offsetAngle), 0, info.normal.z + math.cos(offsetAngle));
        --     dirChangeCounter = DIR_CHANGE_COOLDOWN;
        -- end

        print("dir.x:           " .. dir.x);
        print("dir.z:           " .. dir.z);

        print("target:          " .. targetRotation);
        print("currentRotation: " .. currentRotation);

        if wanderChanges == 3 then
            state = "wait";
            wanderChanges = 0;
        end
    end

    -- print("trans.rot:       " .. entity:get_transform().rot.y);
    rb = entity:get_rigid();
    vel = Vector2.new(rb.linearVelocity.x, rb.linearVelocity.z):magnitude();

    if vel < MAX_SPEED and state ~= "wait" then
        rb:apply_impulse(dir);
    end
end
