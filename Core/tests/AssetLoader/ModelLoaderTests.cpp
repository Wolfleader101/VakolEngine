#include <catch2/catch_all.hpp>

#include <AssetLoader/ModelLoader.hpp>
#include <Rendering/Assets/Model.hpp>

TEST_CASE("Model Loader Tests", "[ModelLoader]")
{
    Vakol::Rendering::Assets::Model model;

    SECTION("Sucessful Import of Cube Model")
    {
        REQUIRE(Vakol::ImportModel(model, "coreAssets/models/cube.obj", 1.0f));
        REQUIRE(model.meshes.size() >= 1);
    }

    SECTION("Successful Import of Sphere Model")
    {
        REQUIRE(Vakol::ImportModel(model, "coreAssets/models/sphere.obj", 1.0f));
        REQUIRE(model.meshes.size() >= 1);
    }

    SECTION("Successful Import of Error Model")
    {
        REQUIRE(Vakol::ImportModel(model, "coreAssets/models/error.obj", 1.0f));
        REQUIRE(model.meshes.size() >= 1);
    }

    SECTION("Handle Non-Existant Models")
    {
        REQUIRE(!Vakol::ImportModel(model, "coreAssets/models/invalid.fbx", 1.0f));
        REQUIRE(model.meshes.size() == 0);
    }   
}