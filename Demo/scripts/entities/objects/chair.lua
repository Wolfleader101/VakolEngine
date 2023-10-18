function init()

	entity:add_model("assets/models/imported/OpenGameArt/loafbrr_1/Furniture/Furniture_Chair_1.fbx", 0.02);

	local rb = entity:add_rigid();

	rb.type = BodyType.Dynamic;

	entity:add_box_collider(Vector3.new(0.7, 1.15, 0.5));
        
    entity:add_script("affordance", "components/affordance.lua")

    local affordComp = entity:get_script("affordance");

    affordComp.AFFORDANCES = {
        SITTABLE = 1.0
    }
end