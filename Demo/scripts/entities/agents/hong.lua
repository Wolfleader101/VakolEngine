local nav;

function init()
    entity:add_model("assets/models/ai/hong/hong.fbx", 1);

    local trans = entity:get_transform();
    trans.pos = Vector3.new(-10, 3, -10);

    trans.scale = Vector3.new(0.015, 0.015, 0.015);

    entity:add_script("emotions", "components/emotion.lua");
    local emotions = entity:get_script("emotions");


    local rb = entity:add_rigid();
    rb.rot_lock = BVector3.new(true, true, true);
    rb.type = BodyType.Dynamic;
    entity:add_box_collider(Vector3.new(0.9, 1.75, 0.3));

    entity:add_script("navigation", "components/navigation.lua");

    nav = entity:get_script("navigation");

    target = scene:get_camera():get_pos();

    nav.TARGET = target;
    nav.MAX_DISTANCE = 0.8;

    nav.MOVE_SPEED = 0.025;
    nav.ROTATE_SPEED = 2.5;
    nav.BRAKE_FORCE = 1.0;

    nav.set_state("chase");
end

function tick()
    target = scene:get_camera():get_pos();
    nav = entity:get_script("navigation");
    -- nav.TARGET = target;

    bin_1 = get_nearby_bins(entity, 100)[1];
    if (bin_1 ~= nil) then
        nav.TARGET = bin_1:get_transform().pos;
    end
    -- print_err(get_nearby_bins(entity, 100)[1]:get_tag())
end

function on_contact(other_ent)
    local affordance = other_ent:get_script("affordance");

    if (affordance ~= nil and affordance.AFFORDANCES.HOLDING == 1.0) then
        print(other_ent:get_tag() .. " Affords Holding")
        if (other_ent:get_script("interactable") ~= nil) then
            other_ent:get_script("interactable").interact(entity);
        end
    end
end

function get_nearby_bins(origin_entity, trigger_distance)
    local origin_pos = origin_entity:get_transform().pos

    local nearby_bins = {}
    local count = 0;
    for i, bin in ipairs(scene.globals.recyclingBins) do
        if bin ~= origin_entity then
            local diff = origin_pos - bin:get_transform().pos
            if diff:magnitude() <= trigger_distance then
                nearby_bins[count + 1] = bin;
                count = count + 1;
            end
        end
    end

    return nearby_bins
end
