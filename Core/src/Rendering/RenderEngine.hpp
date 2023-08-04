#pragma once

#include "RenderAPI.hpp"

#include <memory>

namespace Vakol::Rendering
{
    struct Model;

    enum VK_RENDERDATA_HINT { VK_VERTEX_DATA, VK_SHADER_DATA, VK_TEXTURE_DATA };

    class RenderEngine
    {
    public:
        static void PreDraw();
        static void Draw();
        static void PostDraw();

        static void SubmitModel(const Model& model); // submit an user-defined model to renderer to be broken down to low-level render items
    private:
        /**
         * \brief Submit data to rendering api of user's choosing.
         * \tparam T the type of data to send.
         * \param hint provide a hint to the program which data to be sent.
         * \param data the actual data to be sent.
         */
        template <typename T>
        static void SubmitData(VK_RENDERDATA_HINT hint, T&& data);
    };

    template <typename T>
    void RenderEngine::SubmitData(const VK_RENDERDATA_HINT hint, T&& data)
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
}
