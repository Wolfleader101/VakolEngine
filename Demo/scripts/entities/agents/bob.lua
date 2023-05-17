function init()
    state.fsm = entity:add_fsm();

    state.fsm:add_state("idle", function()
        -- print("Bob is idle")

        local diff = scene.globals.player.pos - entity:get_transform().pos;
        local player_dist = diff:magnitude();
        -- print(diff.x .. " " .. diff.y .. " " .. diff.z)
        print_err(player_dist)


        -- if(playerDist < 10) then
        --     state.fsm:change_state("attacking")
        -- end
        -- if(Input:get_key(KEYS["KEY_A"])) then
        --     state.fsm:change_state("moving")
        -- end
    end)

    state.fsm:add_state("moving", function()
        -- print("Bob is moving")
        -- if(Input:get_key(KEYS["KEY_S"])) then
        --     state.fsm:change_state("idle")
        -- end

    --        -- Transition to the 'attacking' state if an enemy is nearby
    -- if isEnemyNearby() then
    --     state.fsm:change_state("attacking")
    -- end

    -- -- Transition to the 'idle' state if Bob reached his destination
    -- if isDestinationReached() then
    --     state.fsm:change_state("idle")
    -- end
    end)

    state.fsm:add_state("attacking", function()
        print("Bob is attacking")
    end)

    -- Set the initial state
    state.fsm:change_state("idle")

    print_err("Bob is ready")
    scene.globals.bobState = state.fsm;

end


function update()
    state.fsm:update()
end