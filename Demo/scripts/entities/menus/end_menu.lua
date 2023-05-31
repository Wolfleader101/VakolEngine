
function end_menu()
    GUI:start_window("End Menu", 1000.0, 1000.0 , 960 - 500, 540 - 500); 

    GUI:add_text("Thanks for playing", true, false, 2.0, 1.0, 0.0, 0.0, 1.0);
    GUI:add_text(" ", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);
    GUI:add_text(" ", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);

    GUI:add_text("---- Team Vakol ----", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);
    GUI:add_image("assets/textures/vakol.jpg", 500, 500, true, false);

    GUI:add_text(" ", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);
    GUI:add_text("Press to 'Q' to Quit", true, false, 2.0, 1.0, 0.0, 0.0, 1.0);

    GUI:end_window();
    
end

function init()

end

function update()
    end_menu();

    if(Input:get_key_down(KEYS["KEY_Q"])) then
        app_run(false);
    end
end


