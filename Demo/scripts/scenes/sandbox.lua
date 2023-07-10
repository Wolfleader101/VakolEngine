function init()
    scene:create_entity("Player", "entities/player/noclip.lua")
    scene:create_entity("terrain", "entities/terrain/sandboxTerrain.lua");
end

function update()

    print_warn("sandbox")
    if(Input:get_key_down(KEYS["KEY_ESC"])) then
        app_run(false);

    end
end