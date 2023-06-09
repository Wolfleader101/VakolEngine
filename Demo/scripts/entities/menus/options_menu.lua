WINDOWWIDTHMINIMUM = 325.0;
WINDOWWIDTHMAXIMUM = 400.0;

EASY = 1;
MEDIUM = 2;
HARD = 3;

DIFFICULTY = nil;

function set_easy()
    scene.globals.options.DIFFICULTY = EASY;

    scene.globals.options.DROWNING_SPEED_MULTIPLIER = 1.0;
    scene.globals.options.ATTACK_DAMAGE_GIVEN_MULTIPLIER = 1.0;
    scene.globals.options.ATTACK_DAMAGE_DEALT_TO_PLAYER_MULTIPLIER = 1.0;
    scene.globals.options.HEALTH_REGEN_RATE_MULTIPLIER = 1.25;
    scene.globals.options.HUNGER_REGEN_RATE_MULTIPLIER = 1.25;
    scene.globals.options.THIRST_REGEN_RATE_MULTIPLIER = 1.25;
    scene.globals.options.HUNGER_DECREASE_RATE_MULTIPLIER = 1.0;
    scene.globals.options.THIRST_DECREASE_RATE_MULTIPLIER = 1.0;
end

function set_medium()
    scene.globals.options.DIFFICULTY = MEDIUM;

    scene.globals.options.DROWNING_SPEED_MULTIPLIER = 1.5;
    scene.globals.options.ATTACK_DAMAGE_GIVEN_MULTIPLIER = 0.75;
    scene.globals.options.ATTACK_DAMAGE_DEALT_TO_PLAYER_MULTIPLIER = 1.5;
    scene.globals.options.HEALTH_REGEN_RATE_MULTIPLIER = 1.15;
    scene.globals.options.HUNGER_REGEN_RATE_MULTIPLIER = 1.15;
    scene.globals.options.THIRST_REGEN_RATE_MULTIPLIER = 1.15;
    scene.globals.options.HUNGER_DECREASE_RATE_MULTIPLIER = 1.25;
    scene.globals.options.THIRST_DECREASE_RATE_MULTIPLIER = 1.25;
end

function set_hard()
    scene.globals.options.DIFFICULTY = HARD;

    scene.globals.options.DROWNING_SPEED_MULTIPLIER = 2.0;
    scene.globals.options.ATTACK_DAMAGE_GIVEN_MULTIPLIER = 0.65;
    scene.globals.options.ATTACK_DAMAGE_DEALT_TO_PLAYER_MULTIPLIER = 2.0;
    scene.globals.options.HEALTH_REGEN_RATE_MULTIPLIER = 1.05;
    scene.globals.options.HUNGER_REGEN_RATE_MULTIPLIER = 1.05;
    scene.globals.options.THIRST_REGEN_RATE_MULTIPLIER = 1.05;
    scene.globals.options.HUNGER_DECREASE_RATE_MULTIPLIER = 1.5;
    scene.globals.options.THIRST_DECREASE_RATE_MULTIPLIER = 1.5;
end

function get_difficulty()
    if (scene.globals.options.DIFFICULTY == EASY) then
        return 'Tourist';
    elseif (scene.globals.options.DIFFICULTY == MEDIUM) then
        return 'Hiker';
    elseif (scene.globals.options.DIFFICULTY == HARD) then
        return 'Survivalist';
    end
end

function options_menu()
    GUI:change_background_colour(0.349, 0.388, 1.0, 0.9);
    GUI:change_background_rounding(20.0);

    local windowWidth = GUI:get_display_window_width() / 3;

    if windowWidth < WINDOWWIDTHMINIMUM then
        GUI:start_window("Options Menu - Options", true, true, WINDOWWIDTHMINIMUM, 0.0 , 0, 450); 
    elseif windowWidth > WINDOWWIDTHMAXIMUM then
        GUI:start_window("Options Menu - Options", true, true, WINDOWWIDTHMAXIMUM, 0.0 , 0, 450); 
    else
        GUI:start_window("Options Menu - Options", true, true, windowWidth, 0.0 , 0, 450); 
    end

    if (scene.globals.IN_OPTIONS_MENU) then
        GUI:add_text("---- OPTIONS ----", true, false, 3.0, 1.0, 1.0, 1.0, 1.0);
        
        GUI:add_text(" ", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);
        GUI:add_text(" ", true, false, 1.0, 1.0, 1.0, 1.0, 1.0);
    end

    GUI:add_text("Please select a difficulty", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);
    
    if (scene.globals.IN_OPTIONS_MENU) then
        GUI:add_text(" ", true, false, 0.5, 1.0, 1.0, 1.0, 1.0);
    else
        GUI:add_text(" ", true, false, 1.0, 1.0, 1.0, 1.0, 1.0);
    end

    GUI:add_text("---- Difficulty ----", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);

    GUI:add_text(" ", true, false, 1.0, 1.0, 1.0, 1.0, 1.0);
    GUI:add_text("Current Difficulty: " .. get_difficulty(), true, false, 1.5, 1.0, 1.0, 1.0, 1.0);
    GUI:add_text(" ", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);

    GUI:add_button("Tourist (Easy)", true, false, 225.0, 50.0, set_easy);
    GUI:add_button("Hiker (Medium)", true, false, 225.0, 50.0, set_medium);
    GUI:add_button("Survivalist (Hard)", true, false, 225.0, 50.0, set_hard);

    GUI:add_text(" ", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);
    GUI:add_text(" ", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);

    GUI:add_text("Press 'Esc' to return to menu", true, false, 2.0, 1.0, 1.0, 1.0, 1.0);

    GUI:end_window();

    GUI:change_background_colour(0.0, 0.0, 0.0, 0.0);
    GUI:change_background_rounding(0.0);
end

function init()
    scene.globals.options = 
    {
        DIFFICULTY = EASY,
        DROWNING_SPEED_MULTIPLIER = 1.0,
        ATTACK_DAMAGE_GIVEN_MULTIPLIER = 1.0,
        ATTACK_DAMAGE_DEALT_TO_PLAYER_MULTIPLIER = 1.0,
        HEALTH_REGEN_RATE_MULTIPLIER = 1.0,
        HUNGER_REGEN_RATE_MULTIPLIER = 1.0,
        THIRST_REGEN_RATE_MULTIPLIER = 1.0,
        HUNGER_DECREASE_RATE_MULTIPLIER = 1.0,
        THIRST_DECREASE_RATE_MULTIPLIER = 1.0,
        IN_OPTIONS_MENU = false,
    }

    set_wireframe(true);
    set_skybox(false);

    set_active_mouse(true);
end

function update()
    options_menu();

    if(Input:get_key_down(KEYS["KEY_ESC"])) then
        scene:set_active(false);
        set_wireframe(false);
        set_skybox(false);
        local menu = get_scene("Start Scene");
        menu:set_active(true);
        set_active_mouse(false);
    end
end