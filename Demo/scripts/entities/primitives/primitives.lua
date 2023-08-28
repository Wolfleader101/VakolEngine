local sphereEntity;
local sphereModel;

local cubeEntity;
local cubeModel;

function init()
    --sphereEntity = scene:create_entity("Sphere", "");
    cubeEntity = scene:create_entity("Cube", "");

    --Set the sphere parameters
    --local radius = 1.0;
    --local stacks = 100;
    --local sectors = 100;
    --local sphereName = "sphere";
    local cubeName = "cube";

    --Create the sphere
    --Primitives:create_sphere(radius, stacks, sectors, sphereName);

    --Create the cube
    Primitives:create_cube(cubeName);

    --sphereModel = Model.new();

    --Add the sphere model to the scene
    --if(Primitives:get_model(ShapeType.SPHERE, sphereName, sphereModel))
    --then
        --sphereEntity:add_model_direct(sphereModel); 
    --end

    cubeModel = Model.new();

    if(Primitives:get_model(ShapeType.CUBE, cubeName, cubeModel))
    then
        cubeEntity:add_model_direct(cubeModel); 
    end
end

function update()
    
end