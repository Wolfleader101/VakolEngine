function init()
    entity:add_model("assets/models/imported/OpenGameArt/Teh_Bucket/Bench/bench.fbx", 0.02);
    
    local rb = entity:add_rigid();
    rb.mass = 30;
    rb.bounciness = 0.05;
	rb.type = BodyType.Dynamic;
    entity:add_box_collider(Vector3.new(2.64, 1.016, 2.158));
    
    entity:add_script("affordance", "components/affordance.lua")

    local affordComp = entity:get_script("affordance");

    affordComp.AFFORDANCES = {
        SITTABLE = 1.0
    }
end