#include "AssetLoader/ModelLibrary.hpp"
#include "Rendering/Assets/Model.hpp"

#include <catch2/catch_all.hpp>

#include "Utils/GUID.hpp"

using namespace Vakol;

TEST_CASE("ModelLibrary - Model Management", "[ModelLibrary]")
{
    ModelLibrary modelLibrary;

    SECTION("Get and Load Cube Model")
    {
        GUID cubeId; // Generate or assign a GUID for the cube model
        auto& cubeModel = modelLibrary.GetModel(cubeId, "coreAssets/models/cube.obj");
        REQUIRE((cubeModel.meshes.size() >= 1));
    }

    SECTION("Get and Load Sphere Model")
    {
        GUID sphereId; // Generate or assign a GUID for the sphere model
        auto& sphereModel = modelLibrary.GetModel(sphereId, "coreAssets/models/sphere.obj");
        REQUIRE(sphereModel.meshes.size() >= 1);
    }

    SECTION("Get and Load Error Model")
    {
        GUID errorId; // Generate or assign a GUID for the error model
        auto& errorModel = modelLibrary.GetModel(errorId, "coreAssets/models/error.obj");
        REQUIRE(errorModel.meshes.size() >= 1);
    }
}