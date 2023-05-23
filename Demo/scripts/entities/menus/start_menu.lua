
function start_menu()
    GUI:start_window("Start Menu", 500.0, 500.0 , 960 - 250, 540 - 250); 

    GUI:add_text("Welcome to Places Unkown!");

    GUI:add_text("---- MENU ----");

    GUI:add_text("1 - Start/Continue Game");
    GUI:add_text("2 - Options");
    GUI:add_text("ESC - Exit");

    GUI:end_window();
    
end

function init()

    

end


function update()
    start_menu();
    
    if (Input:get_key_down(KEYS["KEY_1"])) then
        scene:set_active(false);

        local game = get_scene("Test Scene");
        game:set_active(true);
    end

    if(Input:get_key_down(KEYS["KEY_ESC"])) then
        scene:set_active(false);

        local endScreen = get_scene("End Scene");
        endScreen:set_active(true);
    end
end


