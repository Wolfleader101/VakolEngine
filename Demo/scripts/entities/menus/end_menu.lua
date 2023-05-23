
function end_menu()
    GUI:start_window("End Menu", 1000.0, 1000.0 , 960 - 500, 540 - 500); 

    GUI:add_text("Thanks for playing");

    GUI:add_text("---- Team Vakol ----");

    GUI:add_image("assets/textures/TeamVakol.jpg", 617, 617);
    GUI:add_text("Q - Exit");

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


