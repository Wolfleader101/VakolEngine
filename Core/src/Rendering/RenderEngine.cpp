#include "RenderEngine.hpp"

#include "Assets/Model.hpp"

#include <View/Window/Window.hpp>

#include <Controller/Logger.hpp>

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

    }

    void RenderEngine::PostDraw()
    {
        
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

    std::shared_ptr<RenderEngine> CreateRenderEngine(const std::string& API, const std::shared_ptr<View::Window>& window)
    {
        return std::make_shared<RenderEngine>();
    }

}
