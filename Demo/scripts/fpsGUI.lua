function update(gui_window)
    gui_window:start_window_creation("FPS Window", 240.0, 240.0, 10.0, 10.0);

    gui_window:add_text("Test String");

    gui_window:end_window_creation();
end