function init()
    local model = entity:add_model("assets/models/Imported/OpenGameArt/yethiel/drinkCup.fbx", 0.035);

    entity:get_transform().pos.y = 1.0;
    
    local rb = entity:add_rigid();
	rb.type = BodyType.Dynamic;
    entity:add_box_collider(model:get_half_extents(0));
    
    entity:add_script("affordance", "components/affordance.lua")

    local affordComp = entity:get_script("affordance");

    affordComp.AFFORDANCES = {
        HOLDING = 1.0,
        THROWING = 1.0,
        RECYCLING = 1.0
    }

    entity:add_script("interactable", "components/interactable.lua");

    local interactable = entity:get_script("interactable");
    interactable.is_throwable = true;
end