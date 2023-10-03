#include <catch2/catch_all.hpp>

#include <AssetLoader/AssetLoader.hpp>

TEST_CASE("AssetLoader Initialization", "[AssetLoader]")
{
    SECTION("Check initial paths")
    {
        REQUIRE(Vakol::AssetLoader::model_path == "assets/models/");
        REQUIRE(Vakol::AssetLoader::texture_path == "assets/textures/");
        REQUIRE(Vakol::AssetLoader::AssetLoader::shader_path == "assets/shaders/");
    }
}

TEST_CASE("Test Something", "[AssetLoader]")
{
    
}