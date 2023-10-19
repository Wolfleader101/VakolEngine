
NAVIGATE = false;
DESTINATION = Vector3.new();
local trans = Transform.new();

local MAX_DIST = 5;
local MAX_SPEED = 5;

local function get_direction()
    return (DESTINATION - trans.pos):normalize();
end

local function distance()
    return (DESTINATION - trans.pos):magnitude();
end

function init()
	
end

function tick()


    if NAVIGATE then


        trans = entity:get_transform();

        dist = distance();

        if dist < MAX_DIST then
            en_route = false;
        else
            dir = get_direction();

            local targetRotation = math.atan(dir.x, dir.z);
            targetRotation = targetRotation * (180 / math.pi);  -- Convert from radians to degrees
            entity:get_transform().rot.y = targetRotation; 

            info = RayCastHitInfo.new();

            if scene:raycast(trans.pos, dir, dist, info) then
                
            end

            
            rb = entity:get_rigid();
            
            vel = Vector2.new(rb.linearVelocity.x, rb.linearVelocity.z):magnitude();
            
            if vel < MAX_SPEED then
                rb:apply_impulse(dir);
            end
        end

    end


end
