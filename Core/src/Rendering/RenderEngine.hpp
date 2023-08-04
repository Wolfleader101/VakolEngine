#pragma once

#include "RenderAPI.hpp"

#include <memory>
#include <string>

namespace Vakol::View
{
    class Window;
}

namespace Vakol::Rendering
{
    namespace Assets
    {
        struct Model;
        struct Mesh;
    }

    class RenderEngine
    {
    public:
        static void PreDraw();
        static void Draw();
        static void PostDraw();

        static void SubmitModel(Assets::Model& model); // submit an user-defined model to renderer to be broken down to low-level render items
    private:
        /**
         * \brief Submit data to rendering api of user's choosing.
         * \tparam T the type of data to send.
         * \param hint provide a hint to the program which data to be sent.
         * \param data the actual data to be sent.
         */
        template <typename T>
        static void SubmitData(VK_RENDER_DATA_HINT hint, T&& data);

        static void SubmitMesh(Assets::Mesh& mesh);
    };

    template <typename T>
    void RenderEngine::SubmitData(const VK_RENDER_DATA_HINT hint, T&& data)
    {
        switch (hint) 
        {
            case VK_VERTEX_DATA:
                RenderAPI::GenerateVertexCommand(std::move(data));
                break;
            case VK_SHADER_DATA:
                RenderAPI::GenerateShaderCommand(std::move(data));
                break;
            case VK_TEXTURE_DATA:
                RenderAPI::GenerateTextureCommand(std::move(data));
                break;
            default:
                break;
        }
    }

    std::shared_ptr<RenderEngine> CreateRenderEngine(const std::string& API, const std::shared_ptr<View::Window>& window);
}
