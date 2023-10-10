#include <catch2/catch_all.hpp>

#include "AssetLoader/AssetLoader.hpp"
#include "Window/Window.hpp"



TEST_CASE("AssetLoader Initialization", "[AssetLoader]")
{
    SECTION("Check initial paths")
    {
        REQUIRE(Vakol::AssetLoader::model_path == "assets/models/");
        REQUIRE(Vakol::AssetLoader::texture_path == "assets/textures/");
        REQUIRE(Vakol::AssetLoader::AssetLoader::shader_path == "assets/shaders/");
    }
}

TEST_CASE("Find Model", "[AssetLoader]")
{
    const Vakol::GUID modelID;

    Vakol::Rendering::Assets::Model model = Vakol::AssetLoader::FindModel(modelID);

    REQUIRE(model.path == "coreAssets/models/error.obj");
    REQUIRE_FALSE(model.meshes.empty());
}

TEST_CASE("Get Model", "[AssetLoader]")
{
    SECTION("Non Existant Model")
    {
        const Vakol::GUID modelID;
        const std::string path;

        Vakol::Rendering::Assets::Model model = Vakol::AssetLoader::GetModel(modelID, path);

        REQUIRE(model.path == "coreAssets/models/error.obj");
        REQUIRE_FALSE(model.meshes.empty());
    }

    SECTION("Existing Model")
    {
        const Vakol::GUID modelID;
        const std::string path = "testAssets/cube.obj";

        Vakol::Rendering::Assets::Model model = Vakol::AssetLoader::GetModel(modelID, path);

        REQUIRE(model.path == path);
        REQUIRE_FALSE(model.meshes.empty());
    }
}

TEST_CASE("Get Meshes", "[AssetLoader]")
{
    // functionally the same as FindModel, we just get the meshes from that found model.

    const Vakol::GUID modelID;

    const std::vector<Vakol::Rendering::Assets::Mesh>& meshes = Vakol::AssetLoader::GetMeshes(modelID);

    REQUIRE_FALSE(meshes.empty());
}

TEST_CASE("Get Texture", "[AssetLoader]")
{
    Vakol::Window window("Test Get Texture (AssetLoader)", 1, 1);

    SECTION("Existing Texture")
    {
        const std::string path = "testAssets/white.png";
        constexpr unsigned int type = Vakol::Rendering::Assets::VK_TEXTURE_DIFFUSE;

        Vakol::Rendering::Assets::Texture texture = Vakol::AssetLoader::GetTexture(path, type);

        REQUIRE(texture.path == path);
        REQUIRE(texture.type == type);
        REQUIRE_FALSE(texture.ID == 0u);
    }

    SECTION("Non-Existant Texture")
    {
        const std::string path;
        constexpr unsigned int type = Vakol::Rendering::Assets::VK_TEXTURE_DIFFUSE;

        Vakol::Rendering::Assets::Texture texture = Vakol::AssetLoader::GetTexture(path, type);

        REQUIRE(texture.path == "coreAssets/textures/error.png");
        REQUIRE(texture.type == type);
        REQUIRE_FALSE(texture.ID == 0u);
    }
}