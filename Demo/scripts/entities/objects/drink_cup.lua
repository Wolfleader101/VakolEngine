function init()
    entity:add_model("assets/models/Imported/OpenGameArt/yethiel/drinkCup.fbx", 1.0);
    
    local rb = entity:add_rigid();
	rb.type = BodyType.Dynamic;
    entity:add_box_collider(Vector3.new(0.074, 0.091, 0.074));
    
    entity:add_script("affordance", "components/affordance.lua")

    local affordComp = entity:get_script("affordance");

    affordComp.AFFORDANCES = {
        PICKUP = 0.5
    }
end