local sphere_guid;

function init()
    --Create a transform for the sphere
    local transform = Transform.new();
    transform.pos = Vector3.new(0, 0, 0);
    transform.scale = Vector3.new(1, 1, 1);
    transform.rot = Vector3.new(0, 0, 0);

    --Set the sphere parameters
    local radius = 10.0;
    local stacks = 20;
    local sectors = 20;
    local name = "sphere";

    --Create the sphere
    Primitives:create_sphere(transform, radius, stacks, sectors, name);

    --Get the guid of the sphere
    sphere_guid = Primitives:get_guid_by_name("sphere")
end

function update()
    Primitives:render(ShapeType.SPHERE, sphere_guid);
end