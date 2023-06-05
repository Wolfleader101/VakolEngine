function init()

    --local ent = scene:create_entity("basicEntity.lua");
    local model = entity:add_model("coreAssets/models/cube.obj", 1 , false, true);

    entity:set_shader("coreAssets/shaders/basic.prog");

    local trans = entity:get_transform();

    local angle = Vector3.new(math.rad(0.0), math.rad(90.0), math.rad(0.0));
    
    trans.pos = Vector3.new(0, 5, -4);
    --trans.rot = Quaternion.new(angle);
    trans.scale = Vector3.new(1, 1, 1);
    
    local r = entity:add_rigid();

    local collider = entity:add_collider();

    collider.Shape = Shape.Box;

    entity:get_bounds_from_model();

    entity:physics_init(scene); 
  
    r:toggle_gravity();

    local vel = Vector3.new(1.0, 1.0, 1.0);

    --r:set_velocity(vel);

    r:set_angular_damp(0);
    
    r:set_angular_velocity(vel);
    

end

function update()
    
end

