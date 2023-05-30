
function end_menu()
    GUI:start_window("End Menu", 1000.0, 1000.0 , 960 - 500, 540 - 500); 

    GUI:add_text("Thanks for playing", true, true, 2.0, 1.0, 0.0, 0.0, 1.0);
    GUI:add_text(" ", true, true, 2.0, 1.0, 1.0, 1.0, 1.0);
    GUI:add_text(" ", true, true, 2.0, 1.0, 1.0, 1.0, 1.0);

    GUI:add_text("---- Team Vakol ----", true, true, 2.0, 1.0, 1.0, 1.0, 1.0);
    GUI:add_image("assets/textures/vakol.jpg", 500, 500, true, true);

    GUI:add_text(" ", true, true, 2.0, 1.0, 1.0, 1.0, 1.0);
    GUI:add_text("Press to 'Q' to Quit", true, true, 2.0, 1.0, 0.0, 0.0, 1.0);

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


