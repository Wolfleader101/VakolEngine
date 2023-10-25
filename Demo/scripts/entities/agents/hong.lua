function init()

    entity:add_model("assets/models/ai/hong/hong.fbx", 1);

    local trans = entity:get_transform();
    trans.pos = Vector3.new(-10, 3, -10); 

    trans.scale = Vector3.new(0.015, 0.015, 0.015);

    entity:add_script("emotions", "components/emotion.lua");
    local emotions = entity:get_script("emotions");

    
    local rb = entity:add_rigid();
    rb.type = BodyType.Static;
    entity:add_box_collider(Vector3.new(0.9, 1.75, 0.3));

end
