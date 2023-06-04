HEALTHMAX = 100;
HUNGERMAX = 75;
THIRSTMAX = 50;

function display_player_stats()
    GUI:change_background_colour(0.349, 0.388, 1.0, 0.9);
    GUI:change_background_rounding(20.0);

    GUI:start_window("Player Stats", false, false, 0.0, 0.0 , 25, 25); 

    GUI:add_image("assets/textures/icons/health/healthIcon_2048.png", 32, 32, false, false);
    GUI:same_line();
    GUI:add_text("Health: " .. string.format("%.f", scene.globals.player.player_health), false, false, 2.0, 1.0, 1.0, 1.0, 1.0);

    GUI:add_image("assets/textures/icons/hunger/hunger.png", 32, 32, false, false);
    GUI:same_line();
    GUI:add_text("Hunger: " .. string.format("%.f", scene.globals.player.player_hunger), false, false, 2.0, 1.0, 1.0, 1.0, 1.0);

    GUI:add_image("assets/textures/icons/thirst/thirst.png", 32, 32, false, false);
    GUI:same_line();
    GUI:add_text("Thirst: " .. string.format("%.f", scene.globals.player.player_thirst), false, false, 2.0, 1.0, 1.0, 1.0, 1.0);

    GUI:end_window();

    GUI:change_background_colour(0.0, 0.0, 0.0, 0.0);
    GUI:change_background_rounding(0.0);
end

function init()
    scene.globals.player.player_health = HEALTHMAX;
    scene.globals.player.player_hunger = HUNGERMAX;
    scene.globals.player.player_thirst = THIRSTMAX;

    scene.globals.player.increment_health = increment_health;
    scene.globals.player.decrement_health = decrement_health;
    scene.globals.player.increment_hunger = increment_hunger;
    scene.globals.player.decrement_hunger = decrement_hunger;
    scene.globals.player.increment_thirst = increment_thirst;
    scene.globals.player.decrement_thirst = decrement_thirst;
end


function increment_health(input_amount)
    if(scene.globals.player.player_health >= 0 and scene.globals.player.player_health < HEALTHMAX) then 
        if((scene.globals.player.player_health + input_amount) > HEALTHMAX) then
            scene.globals.player.player_health = HEALTHMAX;
        else
            scene.globals.player.player_health = scene.globals.player.player_health + input_amount;
        end
    end
end


function decrement_health(input_amount)
    if(scene.globals.player.is_god) then return end;
    if(scene.globals.player.player_health > 0) then 
        if((scene.globals.player.player_health - input_amount) < 0) then
            scene.globals.player.player_health = 0;
        else
            scene.globals.player.player_health = scene.globals.player.player_health - input_amount;
            scene.globals.player.last_damage_time = Time.curr_time;
        end
    end
end

function increment_hunger(input_amount)
    if (scene.globals.player.player_hunger >= 0 and scene.globals.player.player_hunger < HUNGERMAX) then
        if ((scene.globals.player.player_hunger + input_amount) > HUNGERMAX) then
            scene.globals.player.player_hunger = HUNGERMAX;
        else
            scene.globals.player.player_hunger = scene.globals.player.player_hunger + input_amount;
        end
    end
end

function decrement_hunger(input_amount)
    if(scene.globals.player.is_god) then return end;
    if (scene.globals.player.player_hunger > 0) then
        if ((scene.globals.player.player_hunger - input_amount) < 0) then
            scene.globals.player.player_hunger = 0;
        else
            scene.globals.player.player_hunger = scene.globals.player.player_hunger - input_amount;
        end
    end
end

function increment_thirst(input_amount)
    if(scene.globals.player.player_thirst >= 0 and scene.globals.player.player_thirst < THIRSTMAX) then 
        if((scene.globals.player.player_thirst + input_amount) > THIRSTMAX) then
            scene.globals.player.player_thirst = THIRSTMAX;
        else
            scene.globals.player.player_thirst = scene.globals.player.player_thirst + input_amount;
        end
    end
end

function decrement_thirst(input_amount)
    if(scene.globals.player.is_god) then return end;
    if(scene.globals.player.player_thirst > 0) then 
        if((scene.globals.player.player_thirst - input_amount) < 0) then
            scene.globals.player.player_thirst = 0;
        else
            scene.globals.player.player_thirst = scene.globals.player.player_thirst - input_amount;
        end
    end
end


function update()
    display_player_stats();
end