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

        static void SubmitModel(Assets::Model& model); // Submit a user-defined model to renderer. Converted into low-level render components.
    private:
        /**
         * \brief Submit data to rendering api of user's choosing.
         * \tparam T the type of data to send.
         * \param data the actual data to be sent.
         */
        template <typename T>
        static void SubmitVertexData(T&& data);

        static void SubmitMesh(Assets::Mesh& mesh);
    };

    template <typename T>
    void RenderEngine::SubmitVertexData(T&& data)
    {
        RenderAPI::GenerateVertexCommand(std::forward<T>(data));
    }

    std::shared_ptr<RenderEngine> CreateRenderEngine(const std::string& API, const std::shared_ptr<View::Window>& window);
}
