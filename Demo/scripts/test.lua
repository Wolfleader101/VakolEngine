function update()
	GUI:start_window("Test", true, false, 1250.0, 900.0 , 0.0, 100); 

    GUI:add_text("Press 'Esc' to return to menu", true, false, 3.0, 1.0, 1.0, 1.0, 1.0);

    GUI:end_window();
end

function tick()
    print_err(Time.prev_time)
    print(Time.delta_time)

    if(Input:get_key_down(KEYS["KEY_ESC"])) then
        app_run(false);
    end
end