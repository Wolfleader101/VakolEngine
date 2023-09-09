function init()
    entity:add_shaded_model("assets/models/290_level/world.fbx", 50, "assets/shaders/toon.prog");
	
    -- entity:get_transform().pos = Vector3.new(-75, 0, -80);
    -- entity:get_transform().rot = Vector3.new(0, -135, 0);

	entity:get_transform().pos = Vector3.new(234, -168, -751.5);
    entity:get_transform().rot = Vector3.new(0, 0, 0);
end

function update()
end