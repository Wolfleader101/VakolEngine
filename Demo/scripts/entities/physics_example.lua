function init()

    --local ent = scene:create_entity("basicEntity.lua");
    local model = entity:add_model("coreAssets/models/cube.obj", 1 , false, true);

    entity:set_shader("coreAssets/shaders/basic.prog");

    local trans = entity:get_transform();
    
    trans.scale = Vector3.new(1,1,1);

    trans.pos.y = 10;

    
    
    local r = entity:add_rigid();


    local collider = entity:add_collider();

    collider.Shape = Shape.Box;

    entity:get_bounds_from_model();

    entity:physics_init(scene); 

   
    r:toggle_gravity();


    local vel = Vector3.new();

    --vel.x = 1;

    --r:set_velocity(vel);

    vel.x = 0.5;
    --vel.y = 2;
    --vel.z = 0.5;

    --trans.scale = Vector3.new(0.5, 0.5, 0.5);



    r:set_angular_damp(0);
    
    r:set_angular_velocity(vel);
    

end

function update()
    
end

