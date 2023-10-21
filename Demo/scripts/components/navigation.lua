
NAVIGATE = false;
DESTINATION = Vector3.new();
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

function init()
	
end

local WANDER_TICKS = 300; --5 seconds
local wanderCount = 0;


function tick()

    if NAVIGATE then

        trans = entity:get_transform();
        dir = get_direction();
        targetRotation = math.atan(dir.x, dir.z) * (180 / math.pi);  
        entity:get_transform().rot.y = targetRotation; 

        dist = distance();

        if dist < MAX_DIST then
            NAVIGATE = false;
        else

            info = RayCastHitInfo.new();
            

            local rayOrigin = trans.pos + Vector3.new(0, 5, 0);  -- so we dont cast from inside agent collider

            if scene:raycast(rayOrigin, dir, 0.25 , info) and info.distance < CHECK_DIST then
                -- get normal and change dir to follow it. Not the best but will do the job
                local reflectDir = dir - 2 * (dir:dot(info.normal)) * info.normal;
                dir = reflectDir:normalize()
            end
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
