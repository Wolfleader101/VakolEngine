#pragma once

#include "RenderAPI.hpp"

#include <memory>
#include <string>

namespace Vakol::View
{
    class Window;
}

namespace Vakol::Model::Components
{
    struct Transform;
}

namespace Vakol::Rendering
{
    namespace Assets
    {
        struct Model;
        struct Mesh;
    }

    struct Drawable;

    class RenderEngine
    {
    public:
        static void PreDraw();
        static void Draw(Model::Components::Transform& transform, const Drawable& drawable);
        static void PostDraw();

        static void GenerateModel(Assets::Model& model, Drawable& drawable);

        static void GenerateSphere(float scale, Drawable& drawable);
        static void GenerateCube(float scale, Drawable& drawable);
    private:
        /**
         * \brief Submit data to rendering api of user's choosing.
         * \tparam T the type of data to send.
         * \param data the actual data to be sent.
         * \param drawable
         */
        template <typename T>
        static void SubmitVertexData(T&& data, Drawable& drawable);

        template <typename T>
        static void SubmitShaderData(T&& data, Drawable& drawable);

        static void SubmitModel(Assets::Model& model, Drawable& drawable); // Submit a user-defined model to renderer. Converted into low-level render components.
        static void SubmitMesh(Assets::Mesh& mesh, Drawable& drawable);
    };

    template <typename T>
    void RenderEngine::SubmitVertexData(T&& data, Drawable& drawable)
    {
        RenderAPI::GenerateVertexCommand(std::forward<T>(data), drawable);
    }

    template <typename T>
    void RenderEngine::SubmitShaderData(T&& data, Drawable& drawable)
    {
        RenderAPI::GenerateShader(std::forward<T>(data), drawable);   
    }

    std::shared_ptr<RenderEngine> CreateRenderEngine(const std::string& API, const std::shared_ptr<View::Window>& window);
}
