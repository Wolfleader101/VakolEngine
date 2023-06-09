function init()
    scene.globals.player = {
        MAX_HEALTH = 100,
        MAX_HUNGER = 100,
        MAX_THIRST = 100,
        pos = Vector3.new(0.0, 0.0, 0.0),
        last_pos = Vector3.new(0.0, 0.0, 0.0),
        base_speed = 1.25,
        sprint_speed = 3,
        curr_speed = 0.0,
        is_sprinting = false,
        is_god = false,
        health = 0,
        hunger = 0,
        thirst = 0,
        died = false,
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
    if (scene.globals.player.health < 50 and not state.is_drowning and Time.curr_time - scene.globals.player.last_damage_time >= 3) then
        local base_heal_amount = 0.1;
        local player = scene.globals.player;
        
        local hunger_heal_increase = player.hunger / player.MAX_HUNGER;
        local thirst_heal_increase = player.thirst / player.MAX_THIRST;

        local heal_increase = (2/3) * hunger_heal_increase + (1/3) * thirst_heal_increase;

        local heal_amount = base_heal_amount + heal_increase;
        scene.globals.player.increment_health(heal_amount * Time.delta_time);
    end
end

local function update_speed()       
    local player = scene.globals.player;

    local hunger_speed_reduction = (player.MAX_HUNGER - player.hunger) / player.MAX_HUNGER;
    local thirst_speed_reduction = (player.MAX_THIRST - player.thirst) / player.MAX_THIRST;

    local speed_reduction = (1 / 3) * hunger_speed_reduction + (2 / 3) * thirst_speed_reduction; 

    local min_speed = 0.18;
    
    if(player.is_sprinting) then
        local sprint_speed = player.sprint_speed * (1 - speed_reduction);
        scene.globals.player.curr_speed = math.max(min_speed, sprint_speed);
    else
        local walk_speed = player.base_speed * (1 - speed_reduction);
        scene.globals.player.curr_speed = math.max(min_speed, walk_speed);
    end
end

function update()
    if (scene.globals.player.health <= 0.99) then
        scene.globals.player.died = true;
        scene:set_active(false);

        local endScreen = get_scene("End Scene");
        endScreen:set_active(true);
    end

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