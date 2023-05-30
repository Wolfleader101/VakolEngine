function init()

    --local ent = scene:create_entity("basicEntity.lua");
    local model = entity:add_model("coreAssets/models/sphere.obj", 1.0, false, true);

    entity:set_shader("coreAssets/shaders/rainbow.prog");

    local trans = entity:get_transform();
    
    trans.scale.x = 1;
    trans.scale.y = 1;
    trans.scale.z = 1;

    trans.pos.y = 30;

    
    
    local r = entity:add_rigid();

    local rData = rigidData.new();

    

    --rData.angular_lock = phyVec3.new(0, 0, 0);


    local collider = entity:add_collider();

    entity:get_bounds_from_model();

    collider.Shape = Shape.Sphere;
    
    

    entity:physics_init(scene); 
    

    -- print("bruh");
   
    --r:toggle_gravity();

    local vel = Vector3.new();

    --vel.x = 1;

    r:set_velocity(vel);

    

    vel.x = 0.5;
    vel.y = 0.5;
    --vel.z = 0.5;



    r:set_angular_damp(0);
    r:set_angular_velocity(vel);
    
    
    
    print("oof");
    --local trans = entity:get_transform();
    --print("sdf");
    

    
    

end

function update()
    
end

