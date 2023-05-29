function init()
    print("Start Camera Initialized");
    local camera = scene:get_camera();
    local old_pos = camera:get_pos();

    camera:set_pos(old_pos.x , -20, old_pos.z - 100);
    
end

local speed = 0.1;
local sprintSpeed = 50;

function update()
    

    local camera = scene:get_camera();
    local old_pos = camera:get_pos();

    camera:set_pos(old_pos.x, old_pos.y, old_pos.z - speed);

end