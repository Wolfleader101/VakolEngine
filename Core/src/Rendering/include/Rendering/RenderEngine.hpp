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
    struct Vertex;
    struct DebugScene;

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
         * \brief the initial setup of the render engine
         * \param width initial window width
         * \param height initial window height
         * \param API the third-party rendering API chosen by the user (OpenGL, Metal, Direct3D, Vulkan)
         */
        static void Init(int width, int height, const std::string& API);
        /**
         * \brief called before drawing an object
         */
        static void PreDraw();
        /**
         * \brief
         * \param camera the camera of a scene
         * \param transform the transform component of an object.
         * \param drawable the drawable component of an object
         */
        static void Draw(const Camera& camera, const Components::Transform& transform, const Drawable& drawable);
        /**
         * \brief called after drawing an object.
         */
        static void PostDraw();
        /**
         * \brief draws the skybox
         * \param camera the camera of a scene
         * \param skybox the skybox of a scene
         */
        static void DrawSkybox(const Camera& camera, const Skybox& skybox);

        static void DrawDebugScene(const Camera& camera, const DebugScene& debugScene);

        /**
         * \brief generate a model object
         * \param scale the scale of the cube
         * \param drawable the drawable component to be modified with the model's information
         * \param shaderPath
         */
        static void GenerateModel(Assets::Model& model, Drawable& drawable, const std::string& shaderPath);
        /**
         * \brief generate a sphere object
         * \param scale the scale of the sphere
         * \param drawable the drawable component to be modified with the sphere's information
         */
        static void GenerateSphere(float scale, Drawable& drawable);
        /**
         * \brief generate a cube object
         * \param scale the scale of the cube
         * \param drawable the drawable component to be modified with the cube's information
         */
        static void GenerateCube(float scale, Drawable& drawable);

        static void GenerateDebugScene(DebugScene& debugScene);

        /**
         * \brief generate a skybox given the names of the faces of its' cubemap texture.
         * \param faces the names of each face of a skybox cubemap texture
         * \param skybox the skybox of a scene to be modified by the engine.
         */
        static void GenerateSkybox(std::vector<std::string>&& faces, Skybox& skybox);

        /**
         * \brief
         * \param camera the camera of a scene
         * \param width the width of the screen
         * \param height the height of the screen
         */
        static void ResizeScreen(Camera& camera, unsigned int width, unsigned int height);

      private:
        /**
         * \brief generate a vertex array based of the vertices of a skybox
         * \param vertices the vertices of a skybox
         * \param skybox the skybox to be modified by the engine
         */
        static void GenerateSkyboxVertexArray(const std::vector<float>& vertices, Skybox& skybox);

        /**
         * \brief breaks down each mesh of a model.
         * \param model the model to be submitted
         */
        static void SubmitModel(const Assets::Model& model);
        /**
         * \brief converts a high-level mesh object into low-level vertex array object via API
         * \param mesh the mesh to be submitted
         */
        static void SubmitMesh(const Assets::Mesh& mesh);
    };
} // namespace Vakol::Rendering