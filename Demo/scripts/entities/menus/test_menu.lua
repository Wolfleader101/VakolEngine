WINDOWWIDTHMINIMUM = 325.0;
WINDOWWIDTHMAXIMUM = 400.0;

OPENED_OPTIONS_MENU = false;

function start_menu()
    GUI:start_window("Start Menu - Title", true, true); 

    GUI:add_text("Vakol Engine", true, false, 4.0, 1.0, 0.169, 0.169, 1.0);

    GUI:end_window();

    GUI:change_background_colour(0.349, 0.388, 1.0, 0.9);
    GUI:change_background_rounding(20.0);

    local windowWidth = GUI:get_display_window_width() / 3;

    if windowWidth < WINDOWWIDTHMINIMUM then
        GUI:start_window("Start Menu - Options", true, true, WINDOWWIDTHMINIMUM, 160.0, 0.0, 120);
    elseif windowWidth > WINDOWWIDTHMAXIMUM then
        GUI:start_window("Start Menu - Options", true, true, WINDOWWIDTHMAXIMUM, 160.0, 0.0, 120); 
    else
        GUI:start_window("Start Menu - Options", true, true, windowWidth, 160.0, 0.0, 120); 
    end

    GUI:add_text("---- MENU ----", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);

    GUI:add_text("1 - Enter Sandbox", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);

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
        -- toggle_skybox();
        change_scene("sandbox");
        -- toggle_wireframe();
    end

    if(Input:get_key_down(KEYS["KEY_ESC"])) then
        change_scene("end screen");
    end
    
end