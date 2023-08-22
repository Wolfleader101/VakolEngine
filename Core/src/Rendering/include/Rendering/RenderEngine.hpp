#pragma once

#include "RenderAPI.hpp"

#include <memory>
#include <string>

namespace Vakol
{
    class Window;
    class Camera;
} // namespace Vakol

namespace Vakol::Components
{
    struct Transform;
}

namespace Vakol::Rendering
{
    namespace Assets
    {
        struct Model;
        struct Mesh;
    } // namespace Assets

    struct Drawable;

    class RenderEngine
    {
      public:
        static void Init(int width, int height, const std::string& API);

        static void PreDraw();
        static void Draw(const Camera& camera, Components::Transform& transform, const Drawable& drawable);
        static void PostDraw();

        static void GenerateModel(Assets::Model& model, Drawable& drawable);

        static void GenerateSphere(float scale, Drawable& drawable);
        static void GenerateCube(float scale, Drawable& drawable);

      private:
        static void SubmitModel(Assets::Model& model); // Submit a user-defined model to renderer. Converted into
                                                       // low-level render components.
        static void SubmitMesh(Assets::Mesh& mesh);
    };

    std::shared_ptr<RenderEngine> CreateRenderEngine(const std::string& API, const std::shared_ptr<Window>& window);
} // namespace Vakol::Rendering
