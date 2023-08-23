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
    struct Skybox;

    class RenderEngine
    {
      public:
        /**
         * \brief
         * \param width
         * \param height
         * \param API
         */
        static void Init(int width, int height, const std::string& API);
        /**
         * \brief
         */
        static void PreDraw();
        /**
         * \brief
         * \param camera
         * \param transform
         * \param drawable
         */
        static void Draw(const Camera& camera, Components::Transform& transform, const Drawable& drawable);
        /**
         * \brief
         */
        static void PostDraw();
        /**
         * \brief
         * \param camera
         * \param skybox
         */
        static void DrawSkybox(const Camera& camera, const Skybox& skybox);

        /**
         * \brief
         * \param model
         * \param drawable
         */
        static void GenerateModel(Assets::Model& model, Drawable& drawable);
        /**
         * \brief
         * \param scale
         * \param drawable
         */
        static void GenerateSphere(float scale, Drawable& drawable);
        /**
         * \brief
         * \param scale
         * \param drawable
         */
        static void GenerateCube(float scale, Drawable& drawable);

        /**
         * \brief
         * \param faces
         * \param skybox
         */
        static void GenerateSkybox(std::vector<std::string>&& faces, Skybox& skybox);

      private:
        /**
         * \brief
         * \param vertices
         * \param skybox
         */
        static void GenerateSkyboxVertexArray(const std::vector<float>& vertices, Skybox& skybox);

        /**
         * \brief
         * \param model
         */
        static void SubmitModel(Assets::Model& model);
        /**
         * \brief
         * \param mesh
         */
        static void SubmitMesh(Assets::Mesh& mesh);
    };
} // namespace Vakol::Rendering
