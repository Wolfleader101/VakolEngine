function show_start_menu()
    GUI:start_window("Start Menu", 500.0, 500.0, 100.0, 100.0);  -- Modify as needed

    GUI:add_text("Welcome to the Game!");

    -- if GUI:add_button("Start Game") then
    --     return "start"
    -- end

    -- if GUI:add_button("Settings") then
    --     return "settings"
    -- end

    -- if GUI:add_button("Credits") then
    --     return "credits"
    -- end

    -- if GUI:add_button("Exit") then
    --     return "exit"
    -- end

    GUI:end_window();
end

    --GUI:start_frame();
while Input:get_key(KEYS["KEY_1"]) == false do 
    show_start_menu();
    GUI:update();
end

    --GUI:end_frame();

show_start_menu();

add_scene("scenes/testScene.lua", "Test Scene");
--add_scene("scenes/PhysicsScene.lua", "PhysicsScene");
