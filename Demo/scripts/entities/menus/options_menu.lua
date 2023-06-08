WINDOWWIDTHMINIMUM = 325.0;
WINDOWWIDTHMAXIMUM = 400.0;

function options_menu()
    local windowWidth = GUI:get_display_window_width() / 3;
    if windowWidth < WINDOWWIDTHMINIMUM then
        GUI:start_window("Options Menu - Options", true, true, WINDOWWIDTHMINIMUM, 0.0 , 0, 450); 
    elseif windowWidth > WINDOWWIDTHMAXIMUM then
        GUI:start_window("Options Menu - Options", true, true, WINDOWWIDTHMAXIMUM, 0.0 , 0, 450); 
    else
        GUI:start_window("Options Menu - Options", true, true, windowWidth, 0.0 , 0, 450); 
    end

    GUI:add_text("---- OPTIONS ----", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);

    GUI:end_window();
end

function init()

end

function update()
    options_menu();

    if(Input:get_key_down(KEYS["KEY_ESC"])) then
        toggle_wireframe();
        toggle_skybox();
        scene:set_active(false);

        local menu = get_scene("Start Scene");
        menu:set_active(true);
    end
end