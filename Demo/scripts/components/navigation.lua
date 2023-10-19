
local en_route = false;
local dest = Vector3.new();
local trans = Transform.new();

local MAX_DIST = 3;
local speed = 1;

function travel_to(x, y)
    en_route = true;

    --dest = Vector3.new(x, y, z);
    -- dest.x = point.x;
    -- dest.y = point.y;
    -- dest.z = point.z;
    print(x)
    print(y)
    --print(z)

end

local function get_direction()
    print(type(dest));
    print(type(trans.pos));
    return (dest - trans.pos):normalize();
end

local function distance()
    return get_direction():magnitude();
end

function init()
	
end

function tick()


    if en_route then

        
        trans = entity:get_transform();
        

        dist = distance();
        print("here");
        if dist < MAX_DIST then
            en_route = false;
        else
            dir = get_direction();

            desired_rot = math.atan(dir.x, dir.z);

            targetRotation = targetRotation * (180 / math.pi);  -- Convert from radians to degrees
            entity:get_transform().rot.y = targetRotation; 

            

            local newPos = entity:get_transform().pos + direction * speed * Time.delta_time;
            entity:get_transform().pos.x = newPos.x;
            entity:get_transform().pos.z = newPos.z;


            -- info = RayCastHitInfo.new();

            -- if scene:raycast(trans.pos, dir, dist, info) then

            rigid = entity:get_rigid();

            rigid:apply_impulse();
        end

    end


end
