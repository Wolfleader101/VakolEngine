function init()
    
    
end

function update()
    

    if(Input:get_key(KEYS["KEY_5"])) then
        scene:serialize("assets/scenes");
    end

    if(Input:get_key(KEYS["KEY_6"])) then
        scene:deserialize("assets/scenes/".. scene:get_name());
    end

end