function init()
	entity:generate_model("assets/models/agents/monster.fbx", 1.0);

	entity:get_transform().pos = Vector3.new(0.0, -1.0, -5.0);
	entity:get_transform().rot = Vector3.new(0.0, 0.0, 0.0);
end

function update()
end