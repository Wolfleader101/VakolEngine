THROWABLE = 
{
    throw_force = 50
};

local is_held = false;

local rigidbody = nil;
local transform = nil;
local parent_transform = nil;

function init()
    rigidbody = entity:get_rigid();
    transform = entity:get_transform();
end

function interact(parent)
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
    rigidbody:apply_impulse(throwDir * THROWABLE.throw_force);
end

local function hold()
    if (is_held) then
        transform.pos = parent_transform.pos + parent_transform.forward * 2;
    end
end

function update()
    if(is_held) then
        hold()
    end
end