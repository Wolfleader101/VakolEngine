function init()
    state.fsm = entity:add_fsm();


    state.fsm:add_state("idle", function()
        print("Bob is idle")
    end)

    state.fsm:add_state("moving", function()
        print("Bob is moving")

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

end


function update()
    state.fsm:update()
end