function fpsCounter()
    start_window_creation("FPS Window", 240.0, 240.0, 10.0, 10.0);

    add_text(tostring(get_fps()));

    end_window_creation();
end