#include <catch2/catch_all.hpp>

#include <AssetLoader/TextureLoader.hpp>

TEST_CASE("Texture Loader Tests", "[TextureLoader]")
{
    SECTION("Test Error Texture")
    {
        int width = 0, height = 0, channels = 0;
        unsigned char* pixels = nullptr;

        Vakol::ImportTexture("coreAssets/textures/error.png", width, height, channels, pixels);

        REQUIRE(width > 0);
        REQUIRE(height > 0);
        REQUIRE(channels > 0);
        REQUIRE(pixels); // pixels must not be nullptr
    }

    SECTION("Test White Texture")
    {
        int width = 0, height = 0, channels = 0;
        unsigned char* pixels = nullptr;

        Vakol::ImportTexture("coreAssets/textures/white.png", width, height, channels, pixels);

        REQUIRE(width > 0);
        REQUIRE(height > 0);
        REQUIRE(channels > 0);
        REQUIRE(pixels); // pixels must not be nullptr
    }

    SECTION("Test Logo Texture")
    {
        int width = 0, height = 0, channels = 0;
        unsigned char* pixels = nullptr;

        Vakol::ImportTexture("coreAssets/textures/LOGO.png", width, height, channels, pixels);

        REQUIRE(width > 0);
        REQUIRE(height > 0);
        REQUIRE(channels > 0);
        REQUIRE(pixels); // pixels must not be nullptr
    }
}