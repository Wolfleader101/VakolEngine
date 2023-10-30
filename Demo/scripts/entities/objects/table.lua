function init()
    entity:add_model("assets/models/imported/OpenGameArt/loafbrr_1/Furniture/Furniture_Table_2.fbx", 0.02);
    
    local rb = entity:add_rigid();
    rb.mass = 30;
    rb.bounciness = 0.02;
	rb.type = BodyType.Dynamic;
    entity:add_box_collider(Vector3.new(1.8, 1.03, 1.5));
    
    entity:add_script("affordance", "components/affordance.lua")

    local affordComp = entity:get_script("affordance");

    affordComp.AFFORDANCES = {
        
    }
end

