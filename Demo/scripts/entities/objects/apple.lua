function init()
    entity:add_model("assets/models/Imported/OpenGameArt/Teh_Bucket/Apple/apple.fbx", 0.06);
    
    local rb = entity:add_rigid();
	rb.type = BodyType.Dynamic;
    entity:add_sphere_collider(0.2);
    
    entity:add_script("affordance", "components/affordance.lua")

    local affordComp = entity:get_script("affordance");

    affordComp.AFFORDANCES = {
        HOLDING = 1.0,
        THROWING = 1.0,
        TRASHING = 1.0
    }

    entity:add_script("interactable", "components/interactable.lua");

    local interactable = entity:get_script("interactable");
    interactable.is_throwable = true;
end