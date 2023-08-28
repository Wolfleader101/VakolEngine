--local sphere_guid;
local cube_guid;

--local cube;
local cubeEntity;
local cubeModel;

function init()
    --sphere = scene:create_entity("Sphere", "");
    cubeEntity = scene:create_entity("Cube", "");

    --Set the sphere parameters
    --local radius = 5.0;
    --local stacks = 50;
    --local sectors = 50;
    --local sphereName = "sphere";
    local cubeName = "cube";

    --Create the sphere
    --Primitives:create_sphere(radius, stacks, sectors, sphereName);

    --Create the cube
    Primitives:create_cube(cubeName);

    --Get the guid of the sphere
    --sphere_guid = Primitives:get_guid_by_name("sphere");

    --Get the guid of the cube
    cube_guid = Primitives:get_guid_by_name("cube");

    --Add the sphere model to the scene
    --sphere:add_model_direct(Primitives:get_model(ShapeType.SPHERE, sphere_guid));

    cubeModel = Model.new();

    if(Primitives:get_model(ShapeType.CUBE, cube_guid, cubeModel))
    then
        cubeEntity:add_model_direct(cubeModel); 
    end
end

function update()
    
end