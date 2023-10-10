#include <catch2/catch_all.hpp>

#include "Rendering/RenderAPI.hpp"
#include "Rendering/RenderData.hpp"

#include "ECS/Components.hpp"
#include "Utils/GUID.hpp"

#include "Rendering/Assets/Material.hpp"
#include "Rendering/Assets/Shader.hpp"

TEST_CASE("Setup Config", "[RenderAPI]")
{
    constexpr int width = 800;
    constexpr int height = 600;
    const std::string API = "OPENGL";

    Vakol::Rendering::RenderAPI::SetupConfig(width, height, API);

    Vakol::Rendering::RenderConfig config = Vakol::Rendering::RenderAPI::GetConfig();

    REQUIRE(config.width == width);
    REQUIRE(config.height == height);
    REQUIRE(config.API == API);
}

TEST_CASE("Enable Depth Testing", "[RenderAPI]")
{
    Vakol::Rendering::RenderAPI::EnableDepth();
}

TEST_CASE("Enable Multisampling", "[RenderAPI]")
{
    Vakol::Rendering::RenderAPI::EnableMultisample();
}

TEST_CASE("Enable SRGB", "[RenderAPI]")
{
    Vakol::Rendering::RenderAPI::EnableSRGB();
}

TEST_CASE("Enable Blending", "[RenderAPI]")
{
    Vakol::Rendering::RenderAPI::EnableBlending();
}

TEST_CASE("Enable Culling", "[RenderAPI]")
{
    Vakol::Rendering::RenderAPI::EnableCulling();
}

TEST_CASE("Cull Faces", "[RenderAPI]")
{
    Vakol::Rendering::RenderAPI::CullFaces();
}

TEST_CASE("Clear Color", "[RenderAPI]")
{
    constexpr float red = 1.0f;
    constexpr float green = 1.0f;
    constexpr float blue = 1.0f;
    constexpr float alpha = 1.0f;

    SECTION("Invalid Color")
    {
        Vakol::Rendering::RenderAPI::ClearColor(100.0f, -12.0f, blue, alpha);
    }

    SECTION("Valid Color")
    {
        Vakol::Rendering::RenderAPI::ClearColor(red, green, blue, alpha);
    }
}

TEST_CASE("Clear", "[RenderAPI]")
{
    SECTION("Invalid Bitmask")
    {
        Vakol::Rendering::RenderAPI::Clear(1434232532);
    }

    SECTION("Valid Bitmask")
    {
        Vakol::Rendering::RenderAPI::Clear(Vakol::Rendering::VK_DEPTH_BUFFER);
    }
}

TEST_CASE("Resize Screen", "[RenderAPI]")
{
    SECTION("Random Values")
    {
        Vakol::Rendering::RenderAPI::ResizeScreen(-33232, 21000023, -2321, 321);
    }
}

TEST_CASE("Begin Draw", "[RenderAPI]")
{
    const Vakol::GUID modelID;
    const std::string shaderID = "James is Gay";

    Vakol::Rendering::RenderAPI::BeginDraw(modelID, shaderID);
}

TEST_CASE("End Draw", "[RenderAPI]")
{
    Vakol::Rendering::RenderAPI::EndDraw();
}

TEST_CASE("Begin Skybox Draw", "[RenderAPI]")
{
    const std::string vertexID = "i am hungry";
    const std::string shaderID = "i could eat a horse";
    constexpr unsigned int textureID = 420;

    Vakol::Rendering::RenderAPI::BeginSkyboxDraw(vertexID, shaderID, textureID);
}

TEST_CASE("Begin Debug Scene Draw", "[RenderAPI]")
{
    const std::string vertexID = "hi shri!";
    const std::string shaderID = "feelin good, like I should.";

    Vakol::Rendering::RenderAPI::BeginDebugSceneDraw(vertexID, shaderID);
}

TEST_CASE("Get Model Matrix", "[RenderAPI]")
{
    Vakol::Components::Transform transform;

    SECTION("Scale Component at zero")
    {
        constexpr Vakol::Math::Vec3 pos = Vakol::Math::Vec3(1.0f, 1.0f, 1.0f);
        constexpr Vakol::Math::Vec3 scale = Vakol::Math::Vec3(0.0f, 0.0f, 0.0f);

        transform.pos = pos;
        transform.scale = scale;

        Vakol::Math::Mat4 modelMatrix = Vakol::Rendering::RenderAPI::GetModelMatrix(transform);

        REQUIRE(modelMatrix[3][0] == 1.0f);
        REQUIRE(modelMatrix[3][1] == 1.0f);
        REQUIRE(modelMatrix[3][2] == 1.0f);
    }
}

TEST_CASE("Generate Vertex Command (Normal Models)", "[RenderAPI]")
{
    Vakol::Rendering::VertexArray vao;

    Vakol::Rendering::RenderAPI::GenerateVertexCommand(std::move(vao));
}

TEST_CASE("Generate Vertex Command (Skybox)", "[RenderAPI]")
{
    Vakol::Rendering::SkyboxVertexArray vao;

    Vakol::Rendering::RenderAPI::GenerateVertexCommand(std::move(vao));
}

TEST_CASE("Generate Empty Debug Vertex Command (Debug Scene)", "[RenderAPI]")
{
    const Vakol::Rendering::DebugScene debugScene;

    Vakol::Rendering::RenderAPI::GenerateEmptyDebugVertexCommand(debugScene);
}

TEST_CASE("Set Debug Vertex Array", "[RenderAPI]")
{
    std::vector<float> vertices;
    const Vakol::Rendering::DebugScene debugScene;

    Vakol::Rendering::RenderAPI::SetDebugVertexArray(std::move(vertices), debugScene);
}

TEST_CASE("Generate Shader", "[RenderAPI]")
{
    Vakol::Rendering::Assets::Shader shader;
    Vakol::Rendering::Drawable drawable;

    Vakol::Rendering::RenderAPI::GenerateShader(std::move(shader), drawable);

    REQUIRE(drawable.shaderID.empty());
}

TEST_CASE("Generate Skybox Shader", "[RenderAPI]")
{
    Vakol::Rendering::Assets::Shader shader;
    Vakol::Rendering::Skybox skybox;

    Vakol::Rendering::RenderAPI::GenerateSkyboxShader(std::move(shader), skybox);

    REQUIRE(skybox.shaderID.empty());
}

TEST_CASE("Generate Debug Shader", "[RenderAPI]")
{
    Vakol::Rendering::Assets::Shader shader;
    Vakol::Rendering::DebugScene debugScene;

    Vakol::Rendering::RenderAPI::GenerateDebugShader(std::move(shader), debugScene);

    REQUIRE(debugScene.shaderID.empty());
}

TEST_CASE("Generate Texture", "[RenderAPI]")
{
    constexpr int levels = 1;
    constexpr int width = 0;
    constexpr int height = 0;
    constexpr int channels = 1;
    const unsigned char* pixels = nullptr;

    unsigned int texture = Vakol::Rendering::RenderAPI::GenerateTexture(levels, width, height, channels, pixels);

    REQUIRE(pixels == nullptr);
    REQUIRE(texture == 0u);
}

TEST_CASE("Generate Texture (Cubemap)", "[RenderAPI]")
{
    std::vector<std::string> faces;

    const unsigned int texture = Vakol::Rendering::RenderAPI::GenerateTexture(std::move(faces));

    REQUIRE(texture == 0u);
}

TEST_CASE("Add Shader", "[RenderAPI]")
{
    const std::string shaderID = "hello world!";
    constexpr unsigned int shader = 0u;

    Vakol::Rendering::RenderAPI::AddShader(shaderID, shader);
}

TEST_CASE("Get Shader", "[RenderAPI]")
{
    const std::string shaderID = "testing 123";

    const unsigned int shader = Vakol::Rendering::RenderAPI::GetShader(shaderID);

    REQUIRE(shader == 0u);
}