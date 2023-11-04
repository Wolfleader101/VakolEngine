#include <catch2/catch_all.hpp>

#include "AssetLoader/ShaderProcessing.hpp"
#include "Rendering/Assets/Shader.hpp"

Vakol::ShaderProcessing shaderLibrary;

TEST_CASE("Add Shader", "[ShaderLibrary]")
{
    shaderLibrary.AddShader("test", 1);

    REQUIRE_FALSE(shaderLibrary.IsEmpty());
}

TEST_CASE("Get Shader", "[ShaderLibrary]")
{
    SECTION("Existing Shader")
    {
        unsigned int shader = shaderLibrary.GetShader("test");

        REQUIRE_FALSE(shaderLibrary.IsEmpty());
        REQUIRE(shader == 1);
    }

    SECTION("Non-Existant Shader")
    {
        unsigned int shader = shaderLibrary.GetShader("invalid");

        REQUIRE_FALSE(shaderLibrary.IsEmpty());
        REQUIRE(shader == 0);
    }
}