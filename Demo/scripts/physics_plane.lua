function init(scene, entity)

    print("bruh");
    
    local model = entity:add_model("coreAssets/models/cube.obj");

    local trans = entity:get_transform();

    
    
    trans.pos.y = -5;
    trans.scale.x = 100;
    trans.scale.z = 100;

    
    model:set_shader("coreAssets/shaders/basic.prog");
    
    local r = entity:add_rigid();

    r.BodyType = BodyType.Static;

    print("bruh");

    local collider = entity:add_collider();
    
    collider.Shape = Shape.Box;
    
    

    entity:physics_init(scene); 
    
    
    -- 
end

function update(scene, entity)
    
end