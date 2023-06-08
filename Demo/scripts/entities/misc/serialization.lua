function init()
    
    
end

--todo: add terrain to asset loader. key can be scene name

function update()
    

    if(Input:get_key_down(KEYS["KEY_5"])) then
        scene:serialize("assets/scenes");
        print("Checkpoint saved!");
    end

    if(Input:get_key_down(KEYS["KEY_6"])) then
        scene:deserialize("assets/scenes/".. scene:get_name());
        print("Checkpoint loaded!")
    end

end