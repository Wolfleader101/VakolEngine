function init()
    print("Start Camera Initialized");
end

local speed = 10;
local sprintSpeed = 50;

function update()
    

    local camera = scene:get_camera();
    local old_pos = camera:get_pos();

    camera:set_pos(old_pos.x + speed, old_pos.y,old_pos.z);

end