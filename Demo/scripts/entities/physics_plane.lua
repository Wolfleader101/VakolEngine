function init()
    local model = entity:add_model("coreAssets/models/cube.obj", 1.0, false, true);
    entity:set_shader("coreAssets/shaders/basic.prog");
    
    -- local r = entity:add_rigid();


    -- r.BodyType = BodyType.Static;

    

    -- local collider = entity:add_collider();
    
    -- collider.Shape = Shape.Box;
    
    
    -- entity:physics_init(scene); 
    -- --everything above works

    -- r:toggle_gravity();
    
    
end

function update()
    
end