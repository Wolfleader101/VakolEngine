function init(scene, entity)
    
    local model = entity:add_model("coreAssets/models/cube.obj");

    local trans = entity:get_transform();

    
    
    trans.pos.y = -30;
    trans.scale.x = 100;
    trans.scale.z = 100;

    
    model:set_shader("coreAssets/shaders/basic.prog");
    
    local r = entity:add_rigid();


    r.BodyType = BodyType.Static;

    

    local collider = entity:add_collider();
    
    collider.Shape = Shape.Box;
    
    
    entity:physics_init(scene); 
    --everything above works

    r:toggle_gravity();
    
    
end

function update(scene, entity)
    
end