function init()
	local path = "coreAssets/textures/Skybox/";

	local faces = { path .. "right.png", path .. "left.png", path .. "top.png", path .. "bottom.png", path .. "front.png", path .. "back.png"};

	entity:generate_skybox(faces);
end

function update()
end