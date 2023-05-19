
function start_menu()
    GUI:start_window("Start Menu", 500.0, 500.0 , 960 - 250, 540 - 250); 

    GUI:add_text("Welcome to Places Unkown!");

    GUI:add_text("---- MENU ----");

    GUI:add_text("1 - Start Game");
    GUI:add_text("2 - Options");
    GUI:add_text("esc - Exit");

    GUI:end_window();
    
end

function init()

    

end


function update()

    
    if (Input:get_key(KEYS["KEY_1"])) then
        add_scene("scenes/testScene.lua", "Test Scene");
        print("yessir");
        scene:set_active(false);
        print("oof");
    end

    start_menu();

end


