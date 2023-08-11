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

        static void GenerateLine(const Math::Vec3& start, const Math::Vec3& end);

        static void GenerateModel(const char* path, float scale = 1.0f);
        static void GenerateSphere(float scale = 1.0f);
        static void GenerateCube(float scale = 1.0f);
    private:
        /**
         * \brief Submit data to rendering api of user's choosing.
         * \tparam T the type of data to send.
         * \param data the actual data to be sent.
         */
        template <typename T>
        static void SubmitVertexData(T&& data);

        template <typename T>
        static void SubmitShaderData(T&& data);

        static void SubmitModel(Assets::Model& model); // Submit a user-defined model to renderer. Converted into low-level render components.
        static void SubmitMesh(Assets::Mesh& mesh);
    };

    template <typename T>
    void RenderEngine::SubmitVertexData(T&& data)
    {
        RenderAPI::GenerateVertexCommand(std::forward<T>(data));
    }

    template <typename T>
    void RenderEngine::SubmitShaderData(T&& data)
    {
        RenderAPI::GenerateShaderCommand(std::forward<T>(data));   
    }

    std::shared_ptr<RenderEngine> CreateRenderEngine(const std::string& API, const std::shared_ptr<View::Window>& window);
}
