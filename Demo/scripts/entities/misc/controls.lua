function display_controls()

    GUI:change_background_colour(0.349, 0.388, 1.0, 0.9);
    GUI:change_background_rounding(20.0);

	GUI:start_window("Controls Screen", true, true, 1000.0, 1000.0 , 0.0, 100); 

    GUI:add_text("---- CONTROLS ----", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);
    GUI:add_image("assets/textures/controls.png", 1000, 500, true, false);

    GUI:add_text(" ", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);
    GUI:add_text("Press 'Esc' to return to menu", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);

    GUI:end_window();

    GUI:change_background_colour(0.0, 0.0, 0.0, 0.0);
    GUI:change_background_rounding(0.0);
end

function init()

end

function update()
	display_controls();

    if(Input:get_key_down(KEYS["KEY_ESC"])) then
        toggle_wireframe();
        toggle_skybox();
        scene:set_active(false);

        local menu = get_scene("Start Scene");
        menu:set_active(true);
    end
end