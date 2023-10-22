function init()
    entity:add_model("assets/models/Imported/OpenGameArt/LaserDawn/beerCan.fbx", 0.02);
    
    local rb = entity:add_rigid();
	rb.type = BodyType.Dynamic;
    entity:add_box_collider(Vector3.new(0.042, 0.08, 0.042));
    
    entity:add_script("affordance", "components/affordance.lua")

    local affordComp = entity:get_script("affordance");

    affordComp.AFFORDANCES = {
        HOLDING = 1.0,
        THROWING = 1.0,
        RECYCLING = 1.0
    }
end