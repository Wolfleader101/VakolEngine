local emotions;
local navigation;

function init()
    local rb = entity:add_rigid();

    -- entity:add_script("navigation", "components/navigation.lua")
    -- navigation = entity:get_script("navigation");

    entity:add_script("emotions", "fcms/emoiton.lua")
    emotions = entity:get_script("emotions");
end

function move_to_position(target)
    --navigation.DESTINATION = target;
end

function update()
    
end