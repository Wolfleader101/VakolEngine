function start_menu()
    GUI:start_window("Start Menu - Title", true, true, 600.0, 0.0 , 0, 350); 

    GUI:add_text("PLACES UNKNOWN", true, false, 4.0, 1.0, 0.169, 0.169, 1.0);

    GUI:end_window();

    GUI:change_background_colour(0.349, 0.388, 1.0, 0.9);
    GUI:change_background_rounding(20.0);

    GUI:start_window("Start Menu - Options", true, true, 350.0, 0.0 , 0, 450); 

    GUI:add_text("---- MENU ----", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);

    GUI:add_text("1 - Start/Continue Game", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);
    GUI:add_text("2 - Options", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);

    GUI:add_text(" ", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);

    GUI:add_text("ESC - Exit", true, false, 2.0, 1.0, 0.169, 0.169, 1.0);

    GUI:end_window();

    GUI:change_background_colour(0.0, 0.0, 0.0, 0.0);
    GUI:change_background_rounding(0.0);
end

function init()

end

function update()
    start_menu();
    
    if (Input:get_key_down(KEYS["KEY_1"])) then
        toggle_skybox();
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