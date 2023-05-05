function init(scene, entity)

    --local ent = scene:create_entity("basicEntity.lua");
    local model = entity:add_model("coreAssets/models/cube.obj");

    model:set_shader("coreAssets/shaders/rainbow.prog");

    local trans = entity:get_transform();
    
    trans.scale.x = 5;
    trans.scale.y = 5;
    trans.scale.z = 5;



    
    local r = entity:add_rigid();


    local collider = entity:add_collider();

    collider.Shape = Shape.Box;

    entity:physics_init(scene); 
    

    -- print("bruh");
   
    r:toggle_gravity();

    local vel = vec3.new();

    --vel.x = 1;

    r:set_velocity(vel);

    vel.x = 0.5;
    --vel.y = 0.5;
    --vel.z = 0.5;



    r:set_angular_damp(0);
    print("ree");
    r:set_angular_velocity(vel);
    
    
    
    
    --local trans = entity:get_transform();
    --print("sdf");
    

    
    

end

function update(scene, entity)
    
end

