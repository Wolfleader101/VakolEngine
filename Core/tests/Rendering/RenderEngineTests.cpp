#include <catch2/catch_all.hpp>

#include <Rendering/RenderEngine.hpp>
#include <Rendering/Assets/Model.hpp>

#include "Camera/Camera.hpp"
#include "ECS/Components.hpp"

#include "Window/Window.hpp"

#include <Logger/Logger.hpp>


TEST_CASE("Testing Render Engine", "[RenderEngine]")
{
    constexpr int width = 1920;
    constexpr int height = 1080;

    Vakol::Window window("Test Render Engine", width, height);

    SECTION("Test RenderEngine Init")
    {
        const std::string API = "OpenGL";

        Vakol::Rendering::RenderEngine::Init(width, height, API);
        const Vakol::Rendering::RenderConfig config = Vakol::Rendering::RenderEngine::GetAPIConfig();

        REQUIRE(config.width == width);
        REQUIRE(config.height == height);
        REQUIRE(config.API == API);
    }

    SECTION("Test Pre-Draw")
    {
        Vakol::Rendering::RenderEngine::PreDraw();
    }

    SECTION("Test Generation of a Model")
    {
        Vakol::Rendering::Assets::Model model;
        Vakol::Rendering::Drawable drawable;

        Vakol::Rendering::RenderEngine::GenerateModel(model, drawable, "coreAssets/shaders/default.shader");

        REQUIRE(drawable.ID.IsValid());
        REQUIRE(!drawable.shaderID.empty()); // made shaderID a string (classic caleb)
    }

    SECTION("TEST Drawing of a Model")
    {
        Vakol::Rendering::Assets::Model model;
        Vakol::Rendering::Drawable drawable;

        Vakol::Components::Transform transform;

        Vakol::Camera camera;

        Vakol::Rendering::RenderEngine::Draw(camera, transform, drawable);

        // The fact it doesn't break is a testament to my epic programming :p
    }

    SECTION("Test Generation of a Cube")
    {
        Vakol::Rendering::Drawable drawable;
        Vakol::Rendering::RenderEngine::GenerateCube(1.0f, drawable);

        REQUIRE(drawable.ID.IsValid());
        REQUIRE(!drawable.shaderID.empty());
    }

    SECTION("Test Generation of a Sphere")
    {
        Vakol::Rendering::Drawable drawable;
        Vakol::Rendering::RenderEngine::GenerateSphere(1.0f, drawable);

        REQUIRE(drawable.ID.IsValid());
        REQUIRE(!drawable.shaderID.empty());
    }
}