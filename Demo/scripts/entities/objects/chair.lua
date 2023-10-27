function init()
    entity:add_model("assets/models/imported/OpenGameArt/loafbrr_1/Furniture/Furniture_Chair_1.fbx", 0.02);

    local rb = entity:add_rigid();
    rb.mass = 10;
    rb.bounciness = 0.05;
    rb.type = BodyType.Dynamic;

    entity:add_box_collider(Vector3.new(0.7, 1.15, 0.5));

    entity:add_script("affordance", "components/affordance.lua")

    local affordComp = entity:get_script("affordance");

    affordComp.AFFORDANCES = {
        SITTABLE = 1.0,
        THROWING = 1.0,
    }

    entity:add_script("interactable", "components/interactable.lua");

    local interactable = entity:get_script("interactable");
    interactable.is_throwable = true;
end

-- function on_contact(other_ent)
--     if(other_ent:get_rigid().type == BodyType.Static) then
--         return;
--     end
--     print("Chair contacted by " .. other_ent:get_tag());
-- end