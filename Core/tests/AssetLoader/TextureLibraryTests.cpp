#include <catch2/catch_all.hpp>

#include "AssetLoader/TextureLibrary.hpp"
#include "Rendering/Assets/Texture.hpp"

// Create instance of a texture library
Vakol::TextureLibrary textureLibrary;

TEST_CASE("Testing Texture Library", "[TextureLibrary]")
{
    SECTION("Ensure that texture library is empty upon creation")
    {
        REQUIRE(textureLibrary.IsEmpty());
    }
}

TEST_CASE("Get Texture", "[TextureLibrary]")
{
    SECTION("Get Non-Existant Texture")
    {
        const std::string invalidPath = "testAssets/invalid.png";
        constexpr unsigned int type = Vakol::Rendering::Assets::VK_TEXTURE_DIFFUSE;
        const Vakol::Rendering::Assets::Texture& texture = textureLibrary.GetTexture(invalidPath, type);

        REQUIRE_FALSE(textureLibrary.IsEmpty());
        REQUIRE(texture.path == Vakol::ERROR_TEXTURE_PATH);
        REQUIRE(texture.type == type);
        REQUIRE(texture.embedded == false);
    }

    SECTION("Get Non-Existant Embedded Texture")
    {
        const std::string invalidPath = "testAssets/invalid.png";
        constexpr unsigned int type = Vakol::Rendering::Assets::VK_TEXTURE_DIFFUSE;
        constexpr int size = 256;
        const void* data = nullptr;
        const Vakol::Rendering::Assets::Texture& texture = textureLibrary.GetTexture(invalidPath, type, size, data);

        REQUIRE_FALSE(textureLibrary.IsEmpty());
        REQUIRE(texture.path == Vakol::ERROR_TEXTURE_PATH);
        REQUIRE(texture.type == type);
    }

    SECTION("Get Non-Existant Texture Data including its pixels")
    {
        const std::string invalidPath = "testAssets/invalid.png";
        constexpr unsigned int type = Vakol::Rendering::Assets::VK_TEXTURE_DIFFUSE;

        int width = 0;
        int height = 0;
        int channels = 0;
        unsigned char* pixels = nullptr;

        const Vakol::Rendering::Assets::Texture& texture =
            textureLibrary.GetTexture(invalidPath, type, width, height, channels, pixels);

        REQUIRE_FALSE(textureLibrary.IsEmpty());
        REQUIRE_FALSE(texture.width == width);
        REQUIRE_FALSE(texture.height == height);
        REQUIRE_FALSE(texture.channels == channels);
        REQUIRE(pixels == nullptr);
    }
}

TEST_CASE("Get Textures (Cubemap)", "[TextureLibrary]")
{
    std::vector<std::string> input = {"left.png", "right.png", "up.png", "down.png", "back.png", "front.png"};

    std::vector<Vakol::Rendering::Assets::Texture> output;


}