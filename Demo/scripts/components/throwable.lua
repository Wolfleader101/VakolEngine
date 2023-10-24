THROWABLE = 
{
    throw_force = 0.5
};

local is_held = false;
-- local was_held = false;

-- local impulse_applied = false;

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
    -- if (not impulse_applied) then
    print("throw")

    local throwDir = normalize(parent_transform.pos - transform.pos);
    rigidbody:apply_impulse(rad2deg(throwDir) * THROWABLE.throw_force);

        -- impulse_applied = true;
    -- end

    -- impulse_applied = false;
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