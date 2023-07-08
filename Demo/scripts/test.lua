function update()
    print("update")
end

function tick()
    print("tick")
    print_err(Time.prev_time)
    print(Time.delta_time)
end