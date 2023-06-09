WINDOWWIDTHMINIMUM = 325.0;
WINDOWWIDTHMAXIMUM = 400.0;

function easy()
    print("EASY MODE SELECTED");
end

function medium()
    print("MEDIUM MODE SELECTED");
end

function hard()
    print("HARD MODE SELECTED");
end

function options_menu()
    GUI:change_background_colour(0.349, 0.388, 1.0, 0.9);
    GUI:change_background_rounding(20.0);

    local windowWidth = GUI:get_display_window_width() / 3;

    if windowWidth < WINDOWWIDTHMINIMUM then
        GUI:start_window("Options Menu - Options", true, true, WINDOWWIDTHMINIMUM, 0.0 , 0, 450); 
    elseif windowWidth > WINDOWWIDTHMAXIMUM then
        GUI:start_window("Options Menu - Options", true, true, WINDOWWIDTHMAXIMUM, 0.0 , 0, 450); 
    else
        GUI:start_window("Options Menu - Options", true, true, windowWidth, 0.0 , 0, 450); 
    end

    GUI:add_text("---- OPTIONS ----", true, false, 3.0, 1.0, 1.0, 1.0, 1.0);

    GUI:add_text(" ", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);
    GUI:add_text(" ", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);

    GUI:add_text("---- Difficulty ----", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);

    GUI:add_button("Tourist", true, false, 200.0, 50.0, easy);
    GUI:add_button("Hiker", true, false, 200.0, 50.0, medium);
    GUI:add_button("Survivalist", true, false, 200.0, 50.0, hard);

    GUI:add_text(" ", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);
    GUI:add_text(" ", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);

    GUI:add_text("Press 'Esc' to return to menu", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);

    GUI:end_window();

    GUI:change_background_colour(0.0, 0.0, 0.0, 0.0);
    GUI:change_background_rounding(0.0);
end

function init()
    set_active_mouse(true);
end

function update()
    options_menu();

    if(Input:get_key_down(KEYS["KEY_ESC"])) then
        toggle_wireframe();
        toggle_skybox();
        scene:set_active(false);

        local menu = get_scene("Start Scene");
        menu:set_active(true);
        set_active_mouse(false);
    end
end