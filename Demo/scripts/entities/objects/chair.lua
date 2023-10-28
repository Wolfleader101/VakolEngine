function init()
    entity:add_model("assets/models/imported/OpenGameArt/loafbrr_1/Furniture/Furniture_Chair_1.fbx", 0.02);

    local rb = entity:add_rigid();
    -- rb.mass = 10;
    rb.bounciness = 0.05;
    rb.type = BodyType.Dynamic;

    -- base
    entity:add_box_collider(Vector3.new(0.55, 0.1, 0.5), Vector3.new(-0.05, -0.05, 0.0), 4);

    -- back
    entity:add_box_collider(Vector3.new(0.15, 0.6, 0.5), Vector3.new(0.45, 0.55, 0.0), 4);

    -- legs

    -- back right
    entity:add_box_collider(Vector3.new(0.07, 0.55, 0.07), Vector3.new(0.4, -0.6, 0.4), 1);

    -- front right
    entity:add_box_collider(Vector3.new(0.07, 0.55, 0.07), Vector3.new(-0.5, -0.6, 0.4), 1);

    -- back left
    entity:add_box_collider(Vector3.new(0.07, 0.55, 0.07), Vector3.new(0.4, -0.6, -0.4), 1);

    -- front left
    entity:add_box_collider(Vector3.new(0.07, 0.55, 0.07), Vector3.new(-0.5, -0.6, -0.4), 1);

    entity:add_script("affordance", "components/affordance.lua")

    local affordComp = entity:get_script("affordance");

    affordComp.AFFORDANCES = {
        SITTABLE = 1.0,
        HOLDING = 1.0,
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