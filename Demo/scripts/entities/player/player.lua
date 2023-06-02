
function moving_wait(seconds)
    state.MOVING_TIMER = state.MOVING_TIMER + Time.delta_time


    if (state.MOVING_TIMER >= seconds) then
        state.MOVING_TIMER = 0
        return true
    end

    return false
end


function init()
    scene.globals.player = {
        pos = Vector3.new(0.0, 0.0, 0.0),
        last_pos = Vector3.new(0.0, 0.0, 0.0),
        player_health = 0,
        player_hunger = 0,
        player_thirst = 0,
        increment_health = nil,
        decrement_health = nil,
        increment_hunger = nil,
        decrement_hunger = nil,
        increment_thirst = nil,
        decrement_thirst = nil,
    }

    state.moving_value = 0.0;
    state.MOVING_TIMER = 0;

    state.drowning_time = 0;

    scene:create_entity("First Person Camera Controller", "entities/player/firstpersonController.lua");
    scene:create_entity("Player Stats", "entities/player/playerStats.lua");
end

function update()

    if (scene.globals.player.pos.y <= 0.0 and state.drowning_time < 5) then
        state.drowning_time = state.drowning_time + Time.delta_time;
        print(state.drowning_time)
    elseif(scene.globals.player.pos.y > 0.0 and state.drowning_time > 0.0) then
        state.drowning_time = state.drowning_time - Time.delta_time * 2;
        print_err(state.drowning_time)
    end


    if (scene.globals.player.pos.y <= 0.0 and state.drowning_time >= 5) then
        scene.globals.player.decrement_health(10 * Time.delta_time);
    end

    if (moving_wait(10)) then
        scene.globals.player.decrement_hunger(2 * state.moving_value + 0.5);
        scene.globals.player.decrement_thirst(3.5 * state.moving_value + 1.0);

        state.moving_value = 0;
    end
    
    -- if the player's position has changed, they're moving
    if (scene.globals.player.pos ~= scene.globals.player.last_pos) then
        if (state.moving_value >= 1.0) then
            state.moving_value = 1.0;
        else
            state.moving_value = state.moving_value + (0.1 * Time.delta_time);
        end
    end


    -- update the last position to the current position
    scene.globals.player.last_pos = scene.globals.player.pos;
end