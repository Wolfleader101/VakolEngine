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

function lerpAngle(a, b, t)
    local result = a + (math.angleDifference(b, a) * t)
    return result % 360
end

function math.angleDifference(a, b)
    local diff = b - a
    if diff > 180 then
        diff = diff - 360
    elseif diff < -180 then
        diff = diff + 360
    end
    return diff
end

function atan2(y, x)
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

local WANDER_TICKS = 300; -- 3 seconds
local wanderCount = 0;
local wanderChanges = 0; -- used to switch to wait state

local WAIT_TICKS = 600; -- 10 seconds 
local waitCount = 0;

local DIR_CHANGE_COOLDOWN = 300; -- 5 second
local dirChangeCounter = 0;

local currentRotation = 0;
local targetRotation = 0;

local ROTATION_LERP_SPEED = 0.05; -- lerping bruvva

function phys_update()
    trans = entity:get_transform();

    if state == "navigate" then
        dir = get_direction();
        targetRotation = math.atan(dir.x, dir.z) * (180 / math.pi);

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
            local randomX = math.random() -- Random value between 0 and 1
            local randomZ = math.random() -- Random value between 0 and 1
            dir = Vector3.new(randomX, 0, randomZ):normalize()

            wanderCount = 0;
            dirChangeCounter = DIR_CHANGE_COOLDOWN;
            wanderChanges = wanderChanges + 1;

            targetRotation = (atan2(dir.z, dir.x) * (180 / math.pi)) % 360;
            if targetRotation < 0 then
                targetRotation = targetRotation + 360
            end

            currentRotation = targetRotation;
            rb = entity:get_rigid();
            rb.linearVelocity = Vector3.new(0, 0, 0);
        else
            wanderCount = wanderCount + 1;
        end

        -- local info = RayCastHitInfo.new();
        -- local rayOrigin = trans.pos + dir * 0.5;

        -- if scene:raycast(rayOrigin, dir, 0.25, info) then
        --     local offsetAngle = math.random() * 0.5 - 0.25;
        --     dir = Vector3.new(info.normal.x + math.sin(offsetAngle), 0, info.normal.z + math.cos(offsetAngle));
        --     dirChangeCounter = DIR_CHANGE_COOLDOWN;
        -- end

        print("target:          " .. targetRotation);
        print("currentRotation: " .. currentRotation);
        print("trans.rot:       " .. entity:get_transform().rot.y);

        if wanderChanges == 3 then
            state = "wait";
            wanderChanges = 0;
        end
    end

    entity:get_transform().rot.y = currentRotation;
    rb = entity:get_rigid();
    vel = Vector2.new(rb.linearVelocity.x, rb.linearVelocity.z):magnitude();

    if vel < MAX_SPEED and state ~= "wait" then
        rb:apply_impulse(dir);
    end
end
