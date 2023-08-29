#include "Rendering/RenderEngine.hpp"

#include "AssetLoader/ShaderLibrary.hpp"
#include "Rendering/RenderCommand.hpp"

#include "Rendering/Assets/Model.hpp"

#include "AssetLoader/ShaderLoader.hpp"

#include "AssetLoader/AssetLoader.hpp"

#include "Logger/Logger.hpp"

#include "Camera/Camera.hpp"
#include "ECS/Components.hpp"

namespace Vakol::Rendering
{
    const std::vector DEFAULT_CUBE_VERTICES = {
        // positions
        -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
        1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

        -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
        -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

        1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
        1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};

    const std::string DEFAULT_SHADER_PATH = "coreAssets/shaders/default.program";
    const std::string DEFAULT_SKYBOX_SHADER_PATH = "coreAssets/shaders/skybox.program";

    void RenderEngine::Init(const int width, const int height, const std::string& API)
    {
        RenderAPI::SetupConfig(width, height, API);

        RenderAPI::EnableDepth();
        RenderAPI::EnableMultisample();
    }

    void RenderEngine::PreDraw()
    {
        constexpr float color[] = {0.45f, 0.6f, 0.75f};

        RenderAPI::ClearColor(color[0], color[1], color[2]);
        RenderAPI::Clear(VK_COLOR_BUFFER | VK_DEPTH_BUFFER);
    }

    void RenderEngine::Draw(const Camera& camera, Components::Transform& transform, const Drawable& drawable)
    {
        RenderAPI::BeginDraw(drawable.modelID, drawable.shaderID);

        RenderAPI::SetMat4(RenderAPI::GetShader(drawable.shaderID), "PV_MATRIX", false,
                           camera.GetMatrix(PROJECTION_MATRIX) * camera.GetMatrix(VIEW_MATRIX));

        const auto& modelMatrix = RenderAPI::GetModelMatrix(transform);

        RenderAPI::SetMat4(RenderAPI::GetShader(drawable.shaderID), "MODEL_MATRIX", false, modelMatrix);

        RenderAPI::SetMat3(RenderAPI::GetShader(drawable.shaderID), "NORMAL_MATRIX", true,
                           Math::Inverse(Math::Mat3(modelMatrix)));

        RenderAPI::SetVec3(RenderAPI::GetShader(drawable.shaderID), "VIEW_POSITION", camera.GetPos());

        RenderAPI::EndDraw();
    }

    void RenderEngine::PostDraw()
    {
    }

    void RenderEngine::DrawSkybox(const Camera& camera, const Skybox& skybox)
    {
        RenderAPI::BeginSkyboxDraw(skybox.vertexID, skybox.shaderID, skybox.textureID);

        RenderAPI::SetMat4(RenderAPI::GetShader(skybox.shaderID), "PV_MATRIX", false,
                           camera.GetMatrix(PROJECTION_MATRIX) * Math::Mat4(Math::Mat3(camera.GetMatrix(VIEW_MATRIX))));

        RenderAPI::EndSkyboxDraw();
    }

    void RenderEngine::GenerateSphere(const float scale, Drawable& drawable)
    {
        bool success = true;
        auto shader = ImportShader(DEFAULT_SHADER_PATH, success);

        if (success)
            RenderAPI::GenerateShader(std::move(shader), drawable);

        auto model = AssetLoader::GetModel("coreAssets/models/sphere.obj", scale);

        if (success)
            SubmitModel(model);
    }

    void RenderEngine::GenerateCube(const float scale, Drawable& drawable)
    {
        bool success = true;
        auto shader = ImportShader(DEFAULT_SHADER_PATH, success);

        if (success)
            RenderAPI::GenerateShader(std::move(shader), drawable);

        auto model = AssetLoader::GetModel("coreAssets/models/cube.obj", scale);

        if (success)
            SubmitModel(model);
    }

    void RenderEngine::GenerateSkybox(std::vector<std::string>&& faces, Skybox& skybox)
    {
        bool success = true;
        auto shader = ImportShader(DEFAULT_SKYBOX_SHADER_PATH, success);

        if (success)
            RenderAPI::GenerateSkyboxShader(std::move(shader), skybox);

        GenerateSkyboxVertexArray(DEFAULT_CUBE_VERTICES, skybox);

        auto&& textures = AssetLoader::GetTextures(std::move(faces));
        skybox.textureID = RenderAPI::GenerateTexture(std::move(textures));
    }

    void RenderEngine::GenerateSkyboxVertexArray(const std::vector<float>& vertices, Skybox& skybox)
    {
        SkyboxVertexArray vertexArray;

        skybox.vertexID = GenerateID();

        vertexArray.ID = skybox.vertexID;
        vertexArray.vertices = vertices;

        RenderAPI::GenerateVertexCommand(std::move(vertexArray));
    }

    void RenderEngine::GenerateModel(Assets::Model& model, Drawable& drawable)
    {
        bool success = true;
        auto shader = ImportShader(DEFAULT_SHADER_PATH, success);

        if (success)
            RenderAPI::GenerateShader(std::move(shader), drawable);

        drawable.modelID = model.path;

        SubmitModel(model);

        for (auto& mesh : model.meshes)
        {
            const auto& material = mesh.material;

            material->ID = GenerateID();
            material->shaderID = drawable.shaderID;

            auto& textures = material->textures;

            auto compare = [](const Assets::Texture& lhs, const Assets::Texture& rhs) -> bool {
                return lhs.ID < rhs.ID;
            };

            if (textures.empty())
            {
                for (int i = 0; i < 6; ++i)
                {
                    std::string path = "coreAssets/textures/white.png";
                    const unsigned int type = Assets::VK_TEXTURE_DIFFUSE + i;

                    material->textures.emplace_back(AssetLoader::GetTexture(path, type));
                }
            }
            else
            {
                std::string path = "coreAssets/textures/white.png";

                std::vector<unsigned int> types;

                for (const auto& texture : material->textures)
                {
                    types.emplace_back(texture.type);
                }

                for (int i = 0; i < 6; ++i)
                {
                    unsigned int target = Assets::VK_TEXTURE_DIFFUSE + i;

                    if (!std::count(types.begin(), types.end(), target))
                    {
                        const unsigned int type = target;

                        material->textures.emplace_back(AssetLoader::GetTexture(path, type));
                    }
                }
            }

            std::sort(textures.begin(), textures.end(), compare);
        }
    }

    void RenderEngine::SubmitModel(Assets::Model& model)
    {
        for (auto& mesh : model.meshes)
        {
            SubmitMesh(mesh);
        }
    }

    void RenderEngine::SubmitMesh(Assets::Mesh& mesh)
    {
        VertexArray vertexArray;

        vertexArray.ID = mesh.ID;

        vertexArray.vertices = mesh.vertices;
        vertexArray.indices = mesh.indices;

        // swap vector with an empty vector to de-allocate the memory taken by the vector
        // std::vector<unsigned int>().swap(mesh.indices);

        RenderAPI::GenerateVertexCommand(std::move(vertexArray));
    }

} // namespace Vakol::Rendering
