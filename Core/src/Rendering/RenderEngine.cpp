#include "RenderEngine.hpp"

#include "MaterialLibrary.hpp"
#include "RenderCommand.hpp"
#include "ShaderLibrary.hpp"

#include "Assets/Model.hpp"

#include "View/Window/Window.hpp"

#include "Assets/Importer/ShaderImporter.hpp"
#include "Assets/ModelLibrary.hpp"

#include "Controller/Logger.hpp"

#include "Model/Components.hpp"

namespace Vakol::Rendering
{
    const std::string DEFAULT_SHADER_PATH = "coreAssets/shaders/default.program";

    void RenderEngine::PreDraw()
    {
        constexpr float color[] = {0.45f, 0.6f, 0.75f};

        RenderAPI::ClearColor(color);
        RenderAPI::Clear(VK_COLOR_BUFFER | VK_DEPTH_BUFFER);
    }

    void RenderEngine::Draw(Model::Components::Transform& transform, const Drawable& drawable)
    {
        RenderAPI::BeginDraw(drawable.vertexArrayID, drawable.shaderID, drawable.materialID);

        MaterialLibrary::SetupMaterial(MaterialLibrary::GetMaterial(drawable.materialID));
        ShaderLibrary::SetMat4(ShaderLibrary::GetShader(drawable.shaderID), "MODEL_MATRIX", false, RenderAPI::GetModelMatrix(transform));

        RenderAPI::EndDraw();
    }

    void RenderEngine::PostDraw()
    {
        
    }

    void RenderEngine::GenerateSphere(const float scale, Drawable& drawable)
    {
        bool success = true;
        auto shader = Assets::Importer::ImportShader(DEFAULT_SHADER_PATH, success);

        if (success)
            RenderAPI::GenerateShader(std::move(shader), drawable);

        auto model = Assets::ModelLibrary::GetModel("coreAssets/models/sphere.obj", scale);

        if (success)
            SubmitModel(model, drawable);
    }

    void RenderEngine::GenerateCube(const float scale, Drawable& drawable)
    {
        bool success = true;
        auto shader = Assets::Importer::ImportShader(DEFAULT_SHADER_PATH, success);

        if (success)
            RenderAPI::GenerateShader(std::move(shader), drawable);

        auto model = Assets::ModelLibrary::GetModel("coreAssets/models/cube.obj", scale);

        if (success)
            SubmitModel(model, drawable);
    }

    void RenderEngine::GenerateModel(Assets::Model& model, Drawable& drawable)
    {
        bool success = true;
        auto shader = Assets::Importer::ImportShader(DEFAULT_SHADER_PATH, success);

        if (success)
            RenderAPI::GenerateShader(std::move(shader), drawable);

        drawable.vertexArrayID = GenerateID();

        SubmitModel(model, drawable);

        for (auto& mesh : model.meshes)
        {
            auto& material = mesh.material;

            material->ID = GenerateID();
            material->shaderID = drawable.shaderID;

            MaterialLibrary::AddMaterial(*material);

            drawable.materialID = material->ID;

            if (material->textures.empty())
            {
                Assets::Texture texture;

                texture.path = "coreAssets/textures/white.png";
                texture.type = Assets::VK_TEXTURE_DIFFUSE;

                material->textures.emplace_back(texture);
            }

            for (auto& texture : material->textures)
            {
                VK_TRACE(texture.path);

                if (texture.ID == 0)
                    texture.ID = RenderAPI::GenerateTexture(texture, drawable);

                if (texture.ID != 0)
                    MaterialLibrary::AddTexture(material->ID, texture);
            }
        }
    }

    void RenderEngine::SubmitModel(Assets::Model& model, Drawable& drawable)
    {
        RenderAPI::PrepareVertexArray();

        for (auto& mesh : model.meshes) 
        {
            SubmitMesh(mesh, drawable);
        }
    }

    void RenderEngine::SubmitMesh(Assets::Mesh& mesh, Drawable& drawable)
    {
        VertexArray vertexArray;
        
        vertexArray.vertices = mesh.vertices;
        vertexArray.indices = mesh.indices;

        // swap vector with an empty vector to de-allocate the memory taken by the vector
        std::vector<unsigned int>().swap(mesh.indices);

        RenderAPI::GenerateVertexCommand(std::move(vertexArray), drawable);
    }

    std::shared_ptr<RenderEngine> CreateRenderEngine([[maybe_unused]] const std::string& API, const std::shared_ptr<View::Window>& window)
    {
        RenderAPI::EnableDepth();

        return std::make_shared<RenderEngine>();
    }

}
