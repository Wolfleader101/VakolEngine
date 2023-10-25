function init()
    entity:add_model("assets/models/imported/OpenGameArt/PagDev/Trashcan/rubbishBin_Rubbish.fbx", 0.02);

    local rb = entity:add_rigid();

    rb.type = BodyType.Dynamic;

    entity:add_box_collider(Vector3.new(0.6, 1.2, 0.9));

    entity:add_script("affordance", "components/affordance.lua")

    local affordComp = entity:get_script("affordance");

    affordComp.AFFORDANCES = {
    }
end

BIN_CONTENTS = 0

function on_contact(other_ent)
    local affordanceComp = other_ent:get_script("affordance")
    if(affordanceComp ~= nil) then
        if(affordanceComp.AFFORDANCES.TRASHING) then
            scene:destroy_entity(other_ent)
            BIN_CONTENTS = BIN_CONTENTS + 1
        end
    end
end