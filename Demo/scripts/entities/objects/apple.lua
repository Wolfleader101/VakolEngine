function init()
    entity:add_model("assets/models/Imported/OpenGameArt/Teh_Bucket/Apple/apple.fbx", 0.02);
    
    local rb = entity:add_rigid();
	rb.type = BodyType.Dynamic;
    entity:add_sphere_collider(0.032);
    
    entity:add_script("affordance", "components/affordance.lua")

    local affordComp = entity:get_script("affordance");

    affordComp.AFFORDANCES = {
        PICKUP = 0.5
    }
end