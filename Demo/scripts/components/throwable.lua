THROWABLE = 
{
    throw_force = 0.5
};

local is_held = false;
local was_held = false;

local impulse_applied = false;

local rigidbody = nil;
local transform = entity:get_transform();

function init()
    rigidbody = entity:get_rigid();
end

function hold(parent)
    if (is_held) then
        local parent_transform = parent:get_transform();

        transform.pos = parent_transform.pos + parent_transform.forward * 4.0;
    end

    is_held = false;
end

function throw(parent)
    if (not impulse_applied) then
        local parent_transform = parent:get_transform();    

        local throwDir = normalize(parent_transform.pos - transform.pos);
        
        rigidbody:add_impulse(rad2deg(throwDir) * THROWABLE.throw_force);

        impulse_applied = true;
    end

    impulse_applied = false;
end

function update()
    print("test");

    -- Update the previous state
    was_held = is_held;
end