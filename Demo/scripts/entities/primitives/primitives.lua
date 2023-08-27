--local sphere_guid;
local cube_guid;

--local sphere;
local cube;

function init()
    --sphere = scene:create_entity("Sphere", "");
    cube = scene:create_entity("Cube", "");

    --Create a transform for the sphere
    local transform = Transform.new();
    transform.pos = Vector3.new(0, 0, 0);
    transform.scale = Vector3.new(5, 1, 5);
    transform.rot = Vector3.new(0, 0, 0);

    --Set the sphere parameters
    --local radius = 5.0;
    --local stacks = 50;
    --local sectors = 50;
    --local name = "sphere";
    local name = "cube";

    --Create the sphere
    --Primitives:create_sphere(transform, radius, stacks, sectors, name);

    --Create the cube
    Primitives:create_cube(transform, name);

    --Get the guid of the sphere
    --sphere_guid = Primitives:get_guid_by_name("sphere");

    --Get the guid of the cube
    cube_guid = Primitives:get_guid_by_name("cube");

    --Add the sphere model to the scene
    --sphere:add_model_direct(Primitives:get_model(ShapeType.SPHERE, sphere_guid));

    --Add the cube model to the scene
	cube:add_model_direct(Primitives:get_model(ShapeType.CUBE, cube_guid));
end

function update()
    cube:get_transform().rot = Vector3.new(45, 0, 0);
end