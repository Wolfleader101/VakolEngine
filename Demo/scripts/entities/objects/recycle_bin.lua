function init()
    entity:add_model("assets/models/imported/OpenGameArt/PagDev/Trashcan/rubbishBin_Recycling.fbx", 0.02);

    local rb = entity:add_rigid();

    rb.type = BodyType.Static;
    rb.mass = 50;
    rb.rot_lock = BVector3.new(true, true, true);
    rb.bounciness = 0.05;

    entity:add_box_collider(Vector3.new(0.6, 1.2, 0.9));

    entity:add_script("affordance", "components/affordance.lua")

    local affordComp = entity:get_script("affordance");

    affordComp.AFFORDANCES = {
    }

    entity:add_script("bin_content", "components/bin_content.lua")
    
    local binContentComp = entity:get_script("bin_content");
    binContentComp.TYPE = "RECYCLING"
end
