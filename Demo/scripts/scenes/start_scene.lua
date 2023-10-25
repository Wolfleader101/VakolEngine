function init()
    --scene:create_entity("Start Menu", "entities/menus/test_menu.lua");
    scene:create_entity("Player", "entities/player/noclip.lua")
    scene:create_entity("particles", "entities/menus/particles.lua");
    --entity:get_transform().pos = Vector3.new(0.0, 10.0, -7.5);
    --scene:get_camera():set_pos(0, 0, -5);

    local path = "coreAssets/textures/Skybox/";
    local extension = ".png";

    local faces = { path .. "right" .. extension, path .. "left" .. extension, path .. "top" .. extension,
        path .. "bottom" .. extension, path .. "front" .. extension, path .. "back" .. extension };

    scene:generate_skybox(faces);
end

function update()

end
