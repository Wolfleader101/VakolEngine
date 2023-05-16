function init()
    state.x  = 10;
end


function update()
    print(state.x)
    state.x = state.x + 1
    print_err(scene.globals.x )
end