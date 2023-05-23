
function end_menu()
    GUI:start_window("End Menu", 1000.0, 1000.0 , 960 - 500, 540 - 500); 

    GUI:add_text("Thanks for playing");

    GUI:add_text("---- Team Vakol ----");

    GUI:add_image("assets/textures/SampleHeads.png", 500, 500);
    GUI:add_text("Q - Quit");

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


