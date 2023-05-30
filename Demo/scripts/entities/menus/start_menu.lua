function start_menu()
    GUI:start_window("Start Menu", 600.0, 600.0 , 960 - 250, 540 - 250); 

    GUI:add_text("Welcome to", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);
    GUI:add_text("PLACES UNKNOWN", true, false, 4.0, 1.0, 0.0, 0.0, 1.0);
    
    GUI:add_text(" ", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);

    GUI:add_text("---- MENU ----", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);

    GUI:add_text("1 - Start/Continue Game", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);
    GUI:add_text("2 - Options", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);

    GUI:add_text(" ", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);
    GUI:add_text(" ", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);

    GUI:add_text("ESC - Exit", true, false, 2.0, 1.0, 0.0, 0.0, 1.0);

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
        toggle_wireframe();
    end

    if(Input:get_key_down(KEYS["KEY_ESC"])) then
        scene:set_active(false);

        local endScreen = get_scene("End Scene");
        endScreen:set_active(true);
    end
end