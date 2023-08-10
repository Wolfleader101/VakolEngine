#include "RenderEngine.hpp"

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
        RenderAPI::EndDraw();
    }

    void RenderEngine::PostDraw()
    {
        
    }

    void RenderEngine::GenerateSphere()
    {
        bool success = true;

        auto shader = Assets::Importer::ImportShader("coreAssets/shaders/super_basic.prog", success);

        if (success)
            SubmitShaderData(std::move(shader));

        auto model = Assets::Importer::ImportModel("coreAssets/models/sphere.obj", 1.0f);
        SubmitModel(model);
    }

    void RenderEngine::GenerateModel(const char* path, const float scale)
    {
        auto model = Assets::Importer::ImportModel(path, scale);

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
