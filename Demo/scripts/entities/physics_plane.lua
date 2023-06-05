function init()
    
    local model = entity:add_model("coreAssets/models/cube.obj", 1, false, true);

    local trans = entity:get_transform();

    
    
    trans.pos.y = -20;
    trans.scale.x = 100;
    trans.scale.z = 100;

    
    entity:set_shader("coreAssets/shaders/basic.prog");
    
    local r = entity:add_rigid();


    r.BodyType = BodyType.Static;

    

    local collider = entity:add_collider();
    
    collider.Shape = Shape.Box;
    
    
    entity:physics_init(scene); 
    --everything above works

    --r:toggle_gravity();
    
    
end

function update()
    
end