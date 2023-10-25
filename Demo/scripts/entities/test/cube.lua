BLAKE = "IS GAY"

function init()
    print("cube")
    entity:add_model("coreAssets/models/cube.obj", 1);
    entity:get_transform().pos = Vector3.new(0.0, 10.0, -7.5);

    local rb = entity:add_rigid();
    rb.hasGravity = true;

    entity:add_box_collider(entity:get_transform().scale / 2);

    entity:add_script("interactable", "components/interactable.lua");

    local interactable = entity:get_script("interactable");
    interactable.is_throwable = true;
end

function phys_update()
    -- local rb = entity:get_rigid();
    -- if (Input:get_key(KEYS["KEY_1"])) then
    --     rb:add_force(Vector3.new(0.0, 0.0, 100.0));
    -- end
end
