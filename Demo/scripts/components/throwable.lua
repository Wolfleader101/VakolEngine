THROWABLE = 
{
    throw_force = 50
};

local is_held = false;

local rigidbody = nil;
local transform = nil;
local parent_transform = nil;
local ent_parent = nil;

function init()
    rigidbody = entity:get_rigid();
    transform = entity:get_transform();
end

function interact(parent)

    ent_parent = parent;
    parent_transform = parent:get_transform();
    if (not is_held) then
        is_held = true;
    else
        is_held = false;
        throw();
    end
end

function throw()
    local throwDir = parent_transform.forward;
    if(ent_parent:get_tag() == "Player") then 
        throwDir = scene:get_camera():get_forward();
    end
    rigidbody:apply_impulse(throwDir * THROWABLE.throw_force);
end

local lerp_speed = 0.1

local function hold()
    if (is_held) then
        local target_pos
        if (ent_parent:get_tag() == "Player") then
            target_pos = scene:get_camera():get_pos() + scene:get_camera():get_forward() * 4
        else
            target_pos = parent_transform.pos + parent_transform.forward * 4
        end

        transform.pos = transform.pos + (target_pos - transform.pos) * lerp_speed
    end
end


function tick()
    if(is_held) then
        hold()
    end
end