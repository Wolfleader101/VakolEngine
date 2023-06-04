function init()
    scene.globals.player = {
        pos = Vector3.new(0.0, 0.0, 0.0),
        last_pos = Vector3.new(0.0, 0.0, 0.0),
        speed = 1.5,
        sprint_speed = 3.25,
        is_sprinting = false,
        is_god = false,
        player_health = 0,
        player_hunger = 0,
        player_thirst = 0,
        last_damage_time = 0,
        increment_health = nil,
        decrement_health = nil,
        increment_hunger = nil,
        decrement_hunger = nil,
        increment_thirst = nil,
        decrement_thirst = nil,
    }

    state.moving_value = 0.0;
    state.MOVING_TIMER = 0;
    state.REGEN_TIMER = 0;

    state.drowning_time = 0;
    state.is_drowning = false;

    scene:create_entity("First Person Camera Controller", "entities/player/firstpersonController.lua");
    scene:create_entity("Player Stats", "entities/player/playerStats.lua");
end

local function moving_wait(seconds)
    state.MOVING_TIMER = state.MOVING_TIMER + Time.delta_time


    if (state.MOVING_TIMER >= seconds) then
        state.MOVING_TIMER = 0
        return true
    end

    return false
end

local function update_drowning_time()
    if (scene.globals.player.pos.y <= 0.0 and state.drowning_time < 5) then
        state.drowning_time = state.drowning_time + Time.delta_time;
    elseif(scene.globals.player.pos.y > 0.0 and state.drowning_time > 0.0) then
        state.drowning_time = state.drowning_time - Time.delta_time * 2;
    end

end

local function update_moving_value()
    if (scene.globals.player.pos ~= scene.globals.player.last_pos) then
        if (state.moving_value >= 1.0) then
            state.moving_value = 1.0;
        else
            local move_inc = scene.globals.player.is_sprinting and 0.13 or 0.08;
            state.moving_value = state.moving_value + (move_inc * Time.delta_time);
        end
    end
end

local function update_drowning()
    if (scene.globals.player.pos.y <= 0.0 and state.drowning_time >= 2.5) then
        state.is_drowning = true;
        scene.globals.player.decrement_health(15 * Time.delta_time);
    else
        state.is_drowning = false;
    end
end

local function update_regen()
    if (scene.globals.player.player_health < 50 and not state.is_drowning and Time.curr_time - scene.globals.player.last_damage_time >= 3) then
        local base_heal_amount = 0.1;
        local hunger_heal_amount = scene.globals.player.player_hunger > 25 and 0.85 or scene.globals.player.player_hunger > 10 and 0.25 or 0;
        local thirst_heal_amount = scene.globals.player.player_thirst > 15 and 0.4 or scene.globals.player.player_thirst > 8 and 0.15 or 0;
        local heal_amount = base_heal_amount + hunger_heal_amount + thirst_heal_amount;
        scene.globals.player.increment_health(heal_amount * Time.delta_time);
    end
end

local function update_speed()

    local base_speed = 0.65;
    local base_sprint_speed = 2.5;
    local hunger_speed = scene.globals.player.player_hunger > 30 and 0.25 or scene.globals.player.player_hunger > 10 and 0.15 or 0;
    local thirst_speed = scene.globals.player.player_thirst > 35 and 0.65 or scene.globals.player.player_thirst > 25 and 0.3 or 0;

    scene.globals.player.speed = base_speed + hunger_speed + thirst_speed;
    scene.globals.player.sprint_speed = base_sprint_speed + hunger_speed + thirst_speed;
end

function update()
    update_drowning_time()
    update_moving_value()

    if (Input:get_key_down(KEYS["KEY_G"])) then
        scene.globals.player.is_god = not scene.globals.player.is_god;
    end

    if (Input:get_key(KEYS["KEY_T"])) then
        scene.globals.player.increment_health(10 * Time.delta_time)
    end
    
    if (Input:get_key(KEYS["KEY_Y"])) then
        scene.globals.player.decrement_health(10 * Time.delta_time)
    end
    
    if (Input:get_key(KEYS["KEY_U"])) then
        scene.globals.player.increment_hunger(10 * Time.delta_time)
    end
    
    if (Input:get_key(KEYS["KEY_I"])) then
        scene.globals.player.decrement_hunger(10 * Time.delta_time)
    end

    if (Input:get_key(KEYS["KEY_O"])) then
        scene.globals.player.increment_thirst(10 * Time.delta_time)
    end
    
    if (Input:get_key(KEYS["KEY_P"])) then
        scene.globals.player.decrement_thirst(10 * Time.delta_time)
    end


    update_drowning();

    update_regen();

    update_speed();

    if (moving_wait(7.5)) then
        scene.globals.player.decrement_hunger(2 * state.moving_value + 0.25);
        scene.globals.player.decrement_thirst(3.5 * state.moving_value + 0.5);
        state.moving_value = 0;
    end

    

        

    -- update the last position to the current position
    scene.globals.player.last_pos = scene.globals.player.pos;
end