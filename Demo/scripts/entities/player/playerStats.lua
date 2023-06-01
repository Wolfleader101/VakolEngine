HEALTHMAX = 100;
HUNGERMAX = 75;
THIRSTMAX = 50;

function display_player_stats()
    GUI:change_background_colour(0.349, 0.388, 1.0, 0.9);
    GUI:change_background_rounding(20.0);

    GUI:start_window("Player Stats", false, false, 0.0, 0.0 , 25, 25); 

    GUI:add_text("Health: " .. tostring(state.player_health), false, false, 2.0, 1.0, 1.0, 1.0, 1.0);
    GUI:add_text("Hunger: " .. tostring(state.player_hunger), false, false, 2.0, 1.0, 1.0, 1.0, 1.0);
    GUI:add_text("Thirst: " .. tostring(state.player_thirst), false, false, 2.0, 1.0, 1.0, 1.0, 1.0);

    local fps = GUI:get_fps();

    GUI:add_text("GAME FPS: " .. Time.fps, false, false, 1.0, 1.0, 1.0, 1.0, 1.0);

    GUI:end_window();

    GUI:change_background_colour(0.0, 0.0, 0.0, 0.0);
    GUI:change_background_rounding(0.0);
end

function init()
    state.player_health = HEALTHMAX;
    state.player_hunger = HUNGERMAX;
    state.player_thirst = THIRSTMAX;
end

function decrement_health(input_amount)
    if(state.player_health > 0) then 
        if((state.player_health - input_amount) < 0) then
            state.player_health = 0;
        else
            state.player_health = state.player_health - input_amount;
        end
    end
end

function increment_health(input_amount)
    if(state.player_health >= 0 and state.player_health < HEALTHMAX) then 
        if((state.player_health + input_amount) > HEALTHMAX) then
            state.player_health = HEALTHMAX;
        else
            state.player_health = state.player_health + input_amount;
        end
    end
end

function decrement_hunger(input_amount)
    if(state.player_hunger > 0) then 
        if((state.player_hunger - input_amount) < 0) then
            state.player_hunger = 0;
        else
            state.player_hunger = state.player_hunger - input_amount;
        end
    end
end

function increment_hunger(input_amount)
    if(state.player_hunger >= 0 and state.player_hunger < HUNGERMAX) then 
        if((state.player_hunger + input_amount) > HUNGERMAX) then
            state.player_hunger = HUNGERMAX;
        else
            state.player_hunger = state.player_hunger + input_amount;
        end
    end
end

function decrement_thirst(input_amount)
    if(state.player_thirst > 0) then 
        if((state.player_thirst - input_amount) < 0) then
            state.player_thirst = 0;
        else
            state.player_thirst = state.player_thirst - input_amount;
        end
    end
end

function increment_thirst(input_amount)
    if(state.player_thirst >= 0 and state.player_thirst < THIRSTMAX) then 
        if((state.player_thirst + input_amount) > THIRSTMAX) then
            state.player_thirst = THIRSTMAX;
        else
            state.player_thirst = state.player_thirst + input_amount;
        end
    end
end

function update()
    display_player_stats();
end