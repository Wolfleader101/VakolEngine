function init(scene, entity)

    --local ent = scene:create_entity("basicEntity.lua");
    local model = entity:add_model("coreAssets/models/cube.obj");

    model:set_shader("coreAssets/shaders/basic.prog");
    
    local r = entity:add_rigid();

    print("bruh");
    r:toggle_gravity();

    
    
    --local trans = entity:get_transform();
    --print("sdf");
    

    --entity:physics_init(scene); --don't need this and idk why lmao
    

end

function update(scene, entity)
    
end

