function init()
    entity:add_model("assets/models/objects/balls/soccer/soccerBall.fbx", 0.02);
    
    local rb = entity:add_rigid();
	rb.type = BodyType.Dynamic;
    entity:add_sphere_collider(0.23);
    
    entity:add_script("affordance", "components/affordance.lua")

    local affordComp = entity:get_script("affordance");

    affordComp.AFFORDANCES = {
        HOLDING = 1.0,
        THROWING = 1.0
    }
end