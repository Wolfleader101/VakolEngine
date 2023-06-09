add_scene("scenes/start_scene.lua", "Start Scene");

add_scene("scenes/options_scene.lua", "Options Scene")

add_scene("scenes/loading_scene.lua", "Loading Scene")

add_scene("scenes/testScene.lua", "Test Scene");

add_scene("scenes/end_scene.lua", "End Scene");

local options = get_scene("Options Scene");
options:set_active(false);

local loading = get_scene("Loading Scene");
loading:set_active(false);

local game = get_scene("Test Scene");
game:set_active(false);

local endScreen = get_scene("End Scene");
endScreen:set_active(false);

--add_scene("scenes/testSer.lua", "Test Ser");

