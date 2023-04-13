function init(scene)
    print("Hello from TestScene");
    scene:create_entity("cameraController.lua");
    entity = scene:create_entity("player.lua");
    transform = entity:get_transform();
    print_err(transform.pos.y);

end


function update(scene)
    w = Input:get_key(KEYS["KEY_W"])
    if(w)
    then
        print("w is down")
        print(Input:get_mouse_pos().x)
    end
    -- print("Hello from TestScene update!!!");
    -- print(Time["delta_time"]);
end