local sphere_guid;
local sphere;

function init()
    sphere = scene:create_entity("Sphere", "");

    --Create a transform for the sphere
    local transform = Transform.new();
    transform.pos = Vector3.new(0, 0, 0);
    transform.scale = Vector3.new(1, 1, 1);
    transform.rot = Vector3.new(0, 0, 0);

    --Set the sphere parameters
    local radius = 5.0;
    local stacks = 50;
    local sectors = 50;
    local name = "sphere";

    --Create the sphere
    Primitives:create_sphere(transform, radius, stacks, sectors, name);

    --Get the guid of the sphere
    sphere_guid = Primitives:get_guid_by_name("sphere");

    --Add the sphere model to the scene
    sphere:add_model_direct(Primitives:get_model(ShapeType.SPHERE, sphere_guid));
end

function update()
    
end