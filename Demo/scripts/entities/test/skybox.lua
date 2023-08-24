function init()
	local path = "coreAssets/textures/Skybox/imported/Unity/WorldSkies/";
	local extension = ".png";

	local faces = { path .. "left"  .. extension, 
					path .. "right"   .. extension, 
					path .. "top"    .. extension, 
					path .. "bottom" .. extension, 
					path .. "front"  .. extension, 
					path .. "back"   .. extension};

	scene:generate_skybox(faces);
end

function update()
end