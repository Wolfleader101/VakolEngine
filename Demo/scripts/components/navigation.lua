
NAVIGATE = false;
DESTINATION = Vector3.new();
local trans = Transform.new();
local dir = Vector3.new();

local CHECK_DIST = 5
local MAX_DIST = 5;
MAX_SPEED = 3;

local function get_direction()
    return (DESTINATION - trans.pos):normalize();
end

local function distance()
    return (DESTINATION - trans.pos):magnitude();
end

function init()
	
end

local waypoints = {};
local WANDER_TICKS = 300; --5 seconds
local wanderCount = 0;

function tick()

    if NAVIGATE then

        trans = entity:get_transform();
        dir = (DESTINATION - trans.pos):normalize();
        targetRotation = math.atan(dir.x, dir.z) * (180 / math.pi);  
        entity:get_transform().rot.y = targetRotation; 

        dist = distance();

        if dist < MAX_DIST then
            NAVIGATE = false;
        else

            info = RayCastHitInfo.new();
            leftInfo = RayCastHitInfo.new();
            rightInfo = RayCastHitInfo.new();

            -- if scene:raycast(trans.pos, dir, dist, info) then
            --     slideDir = Vector3.new(-info.normal.z, 0, info.normal.x);
            --     dir = slideDir;
            -- else
            --     leftDir = Vector3.new(-dir.z, 0, dir.x);
            --     rightDir = Vector3.new(dir.z, 0, -dir.x);
            --     leftInfo = RayCastHitInfo.new();
            --     rightInfo = RayCastHitInfo.new();
    
            --     if scene:raycast(trans.pos, leftDir, CHECK_DIST, leftInfo) then
            --         slideDirLeft = Vector3.new(-leftInfo.normal.z, 0, leftInfo.normal.x);
            --         dir = slideDirLeft;
            --     elseif scene:raycast(trans.pos, rightDir, CHECK_DIST, rightInfo) then
            --         slideDirRight = Vector3.new(-rightInfo.normal.z, 0, rightInfo.normal.x);
            --         dir = slideDirRight;
            --     end
            -- end
        end

    else --wander state

        
        if wanderCount >= WANDER_TICKS then
            local randomAngle = math.random() * 2 * math.pi;
            dir = Vector3.new(math.cos(randomAngle), 0, math.sin(randomAngle));
            wanderCount = 0;
        else
            wanderCount = wanderCount + 1;
        end
    
        targetRotation = math.atan(dir.x, dir.z) * (180 / math.pi);
        entity:get_transform().rot.y = targetRotation;
    
        info = RayCastHitInfo.new();
    
        -- If there's an obstacle in the wander direction, slide along it
        if scene:raycast(trans.pos, dir, CHECK_DIST, info) and info.hit then
            slideDir = Vector3.new(-info.normal.z, 0, info.normal.x);
            dir = slideDir;
        end
    
        
    end

        rb = entity:get_rigid();
        vel = Vector2.new(rb.linearVelocity.x, rb.linearVelocity.z):magnitude();

        if vel < MAX_SPEED then
            rb:apply_impulse(dir);
        end


end
