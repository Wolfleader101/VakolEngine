

function init()
    
    entity:add_model("assets/models/agents/bob.glb", 1.0e-02);
    entity:get_transform().pos = Vector3.new(-40, 5, -40)

    local rb = entity:add_rigid();
	rb.mass = 1;
    entity:add_box_collider(Vector3.new(1.0, 1.0, 1.0));

    entity:add_script("emotions", "components/emotion.lua");

    entity:add_script("navigation", "components/navigation.lua");
    local nav = entity:get_script("navigation");

    --target = Vector3.new(40, 1, 40);
    nav:travel_to(40.0, 1.0);
    	
    
end