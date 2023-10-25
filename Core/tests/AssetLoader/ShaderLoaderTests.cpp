#include <catch2/catch_all.hpp>

#include <AssetLoader/ShaderLoader.hpp>

TEST_CASE("Shader Loader Testing", "[ShaderLoader]")
{
    SECTION("Test Debug Shader")
    {
        bool success;

        Vakol::ImportShader("coreAssets/shaders/debug.shader", success);

        REQUIRE(success);
    }

    SECTION("Test Default Shader")
    {
        bool success;

        Vakol::ImportShader("coreAssets/shaders/default.shader", success);

        REQUIRE(success);
    }

    SECTION("Test Skybox Shader")
    {
        bool success;

        Vakol::ImportShader("coreAssets/shaders/skybox.shader", success);

        REQUIRE(success);
    }
}