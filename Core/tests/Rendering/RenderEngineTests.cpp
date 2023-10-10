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
    Vakol::Camera camera {Vakol::Math::Vec3(0.0f)};

    SECTION("Test RenderEngine Init")
    {
        const std::string API = "OpenGL";

        Vakol::Rendering::RenderEngine::Init(width, height, API);
        const Vakol::Rendering::RenderConfig config = Vakol::Rendering::RenderEngine::GetAPIConfig();

        REQUIRE(config.width == width);
        REQUIRE(config.height == height);
        REQUIRE(config.API == API);
    }

    SECTION("Test Resize Screen")
    {
        constexpr unsigned int resizeWidth = 800;
        constexpr unsigned int resizeHeight = 600;

        Vakol::Rendering::RenderEngine::ResizeScreen(camera, resizeWidth, resizeHeight);
        REQUIRE(camera.GetAspect() == static_cast<float>(resizeWidth) / static_cast<float>(resizeHeight));
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

    SECTION("Test Drawing of a Model")
    {
        Vakol::Rendering::Assets::Model model;
        Vakol::Rendering::Drawable drawable;

        Vakol::Components::Transform transform;

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

    SECTION("Test Drawing of a Skybox")
    {
        Vakol::Rendering::Skybox skybox;

        Vakol::Rendering::RenderEngine::DrawSkybox(camera, skybox);
    }

    SECTION("Test Drawing of a Debug Scene")
    {
        Vakol::Rendering::DebugScene debugScene;

        Vakol::Rendering::RenderEngine::DrawDebugScene(camera, debugScene);
    }
}