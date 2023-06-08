function init()

    --local ent = scene:create_entity("basicEntity.lua");
    local model = entity:add_model("assets/models/cube.obj", 1 , false, true);

    entity:set_shader("coreAssets/shaders/basic.prog");

    local trans = entity:get_transform();
    
    trans.pos = Vector3.new(0, 5, -4);
    trans.rot = Vector3.new(120.0, 90.0, 0.0)
    
    local r = entity:add_rigid();

    local collider = entity:add_collider();

    collider.Shape = Shape.Box;

    entity:get_bounds_from_model();

    entity:physics_init(scene); 
  
    r:toggle_gravity();

    --local vel = Vector3.new(0.0, 0.0, 0.0);
    local vel = Vector3.new(2.0, 1.0, 0.0);

    --r:set_velocity(vel);

    r:set_angular_damp(0);
    
    r:set_angular_velocity(vel);
    

end

function update()
    
end

