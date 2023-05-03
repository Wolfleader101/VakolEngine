function init(scene, entity)

    --local ent = scene:create_entity("basicEntity.lua");
    local model = entity:add_model("coreAssets/models/cube.obj");

    model:set_shader("coreAssets/shaders/basic.prog");
    
    local r = entity:add_rigid();

    --local r = entity:get_rigid();
    entity:physics_init(scene); 

    print("bruh");
   
    r:toggle_gravity();

    local vel = vec3.new();

    vel.x = 1;

    r:set_velocity(vel);

    vel.x = 60;
    vel.z = 40;

    r:set_angular_velocity(vel);
    

    
    
    --local trans = entity:get_transform();
    --print("sdf");
    

    
    

end

function update(scene, entity)
    
end

