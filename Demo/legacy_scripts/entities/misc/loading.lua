function loading_screen()
	GUI:start_window("Loading Screen", true, true, 1000.0, 1000.0 , 0.0, 100);
	GUI:add_text("Loading Game Scene: Please Wait", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);
	GUI:end_window();
end

function init()
	--game = get_scene("Test Scene");
	--game:set_active(true);
end

function update()
	loading_screen();

end