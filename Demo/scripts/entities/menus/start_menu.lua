WINDOWWIDTHMINIMUM = 325.0;
WINDOWWIDTHMAXIMUM = 400.0;

OPENED_OPTIONS_MENU = false;

function start_menu()
    GUI:start_window("Start Menu - Title", true, true, 700, 0.0 , 0, 350); 

    GUI:add_text("PLACES UNKNOWN", true, false, 4.0, 1.0, 0.169, 0.169, 1.0);

    GUI:end_window();

    GUI:change_background_colour(0.349, 0.388, 1.0, 0.9);
    GUI:change_background_rounding(20.0);

    local windowWidth = GUI:get_display_window_width() / 3;
    if windowWidth < WINDOWWIDTHMINIMUM then
        GUI:start_window("Start Menu - Options", true, true, WINDOWWIDTHMINIMUM, 0.0 , 0, 450); 
    elseif windowWidth > WINDOWWIDTHMAXIMUM then
        GUI:start_window("Start Menu - Options", true, true, WINDOWWIDTHMAXIMUM, 0.0 , 0, 450); 
    else
        GUI:start_window("Start Menu - Options", true, true, windowWidth, 0.0 , 0, 450); 
    end

    GUI:add_text("---- MENU ----", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);

    GUI:add_text("1 - Start/Continue Game", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);
    GUI:add_text("2 - Options", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);
    GUI:add_text("3 - Controls", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);

    GUI:add_text(" ", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);

    GUI:add_text("ESC - Exit", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);

    GUI:end_window();

    GUI:change_background_colour(0.0, 0.0, 0.0, 0.0);
    GUI:change_background_rounding(0.0);
end

function init()

end

function update()
    start_menu();
    
    if (Input:get_key_down(KEYS["KEY_1"])) then
        scene:set_active(false);

        local game = get_scene("Nordic World");
        game:set_active(true);
        set_skybox(true);
    end

    if (Input:get_key_down(KEYS["KEY_2"])) then
        scene:set_active(false);

        local options_scene = get_scene("Options Scene");
        options_scene:set_active(true);
        options_scene.globals.options.OPENED_OPTIONS_MENU = true;
    end

    if (Input:get_key_down(KEYS["KEY_3"])) then
        scene:set_active(false);

        local controls = get_scene("Controls Scene");
        controls:set_active(true);
    end

    if(Input:get_key_down(KEYS["KEY_ESC"])) then
        scene:set_active(false);

        local endScreen = get_scene("End Scene");
        endScreen:set_active(true);
    end
end