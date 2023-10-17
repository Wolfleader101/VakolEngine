function init()
    entity:add_model("assets/models/objects/balls/soccer/soccerBall.fbx", 1.0);
    
    local rb = entity:add_rigid();
	rb.type = BodyType.Dynamic;
    entity:add_sphere_collider(0.11);
    
    entity:add_script("affordance", "components/affordance.lua")

    local affordComp = entity:get_script("affordance");

    affordComp.AFFORDANCES = {
        PICKUP = 1.0
    }
end