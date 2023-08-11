#include "RenderEngine.hpp"

#include "MaterialLibrary.hpp"
#include "ShaderLibrary.hpp"

#include "Assets/Model.hpp"

#include "View/Window/Window.hpp"

#include "Assets/Importer/ShaderImporter.hpp"
#include "Assets/Importer/ModelImporter.hpp"
#include "Controller/Logger.hpp"

namespace Vakol::Rendering
{
    void RenderEngine::PreDraw()
    {
        constexpr float color[] = {0.45f, 0.6f, 0.75f};

        RenderAPI::ClearColor(color);
        RenderAPI::Clear(VK_COLOR_BUFFER | VK_DEPTH_BUFFER);
    }

    void RenderEngine::Draw()
    {
        RenderAPI::BeginDraw();

        MaterialLibrary::SetColor(ShaderLibrary::GetShader("coreAssets/shaders/primitive.prog"), Math::Vec4(1.0f, 1.0f, 1.0f, 1.0f));

        RenderAPI::EndDraw();
    }

    void RenderEngine::PostDraw()
    {
        
    }

    void RenderEngine::GenerateLine(const Math::Vec3& start, const Math::Vec3& end)
    {
        bool success = true;
        auto shader = Assets::Importer::ImportShader("coreAssets/shaders/primitive.prog", success);

        if (success)
            SubmitShaderData(std::move(shader));

        RenderAPI::GenerateVertexArrayFromLine(start, end);
    }

    void RenderEngine::GenerateSphere(const float scale)
    {
        bool success = true;
        auto shader = Assets::Importer::ImportShader("coreAssets/shaders/primitive.prog", success);

        if (success)
            SubmitShaderData(std::move(shader));

        auto model = Assets::Importer::ImportModel("coreAssets/models/sphere.obj", scale, success);

        if (success)
            SubmitModel(model);
    }

    void RenderEngine::GenerateCube(const float scale)
    {
        bool success = true;
        auto shader = Assets::Importer::ImportShader("coreAssets/shaders/primitive.prog", success);

        if (success)
            SubmitShaderData(std::move(shader));

        auto model = Assets::Importer::ImportModel("coreAssets/model/cube.obj", scale, success);

        if (success)
            SubmitModel(model);
    }

    void RenderEngine::GenerateModel(const char* path, const float scale)
    {
        bool success = true;
        auto model = Assets::Importer::ImportModel(path, scale, success);

        if (success)
            SubmitModel(model);
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
        
        vertexArray.vertices = mesh.vertices;
        vertexArray.indices = mesh.indices;

        // swap vector with an empty vector to de-allocate the memory taken by the vector
        std::vector<unsigned int>().swap(mesh.indices);

        SubmitVertexData(std::move(vertexArray));
    }

    std::shared_ptr<RenderEngine> CreateRenderEngine([[maybe_unused]] const std::string& API, const std::shared_ptr<View::Window>& window)
    {
        return std::make_shared<RenderEngine>();
    }

}
