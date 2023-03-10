#include "GLRenderer.hpp"

#include <glad/glad.h>

#include <Model/Math/Vector3.hpp>
#include <Model/Math/Vector2.hpp>

#include <vector>

using Vakol::Model::GLMesh;

using Vakol::Model::GLVertexArray;

using Vakol::Model::Vertex3;
using Vakol::Model::Vertex2N;
using Vakol::Model::Vertex2U;

using Vakol::Model::GLShader;
using Vakol::Model::GLMaterial;

using Vakol::Model::Math::Vec3;
using Vakol::Model::Math::Vec2;

namespace Vakol::View 
{
    GLRenderer::GLRenderer(const std::shared_ptr<Window> window) : Renderer(window) 
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);

        GLShader shader("assets/skybox.vert", "assets/skybox.frag");

        // std::vector<Vertex3> vertices = { 
        //     {
        //         {Vec3( 0.5f,   0.5f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec2(1.0f, 1.0f)},
        //         {Vec3( 0.5f,  -0.5f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec2(1.0f, 0.0f)},
        //         {Vec3(-0.5f,  -0.5f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec2(0.0f, 0.0f)},
        //         {Vec3(-0.5f,   0.5f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec2(0.0f, 1.0f)}
        //     }
        // };

        std::vector<float> vertices = 
        {
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
        };

        // std::vector<Vec3> vertices = 
        // {
        //     // Front
        //     Vec3(-1.0f, -1.0f, 1.0f),
        //     Vec3( 1.0f, -1.0f, 1.0f),
        //     Vec3( 1.0f,  1.0f, 1.0f),
        //     Vec3(-1.0f,  1.0f, 1.0f),

        //     // Back
        //     Vec3(-1.0f, -1.0f,  -1.0f),
        //     Vec3( 1.0f, -1.0f,  -1.0f),
        //     Vec3( 1.0f,  1.0f,  -1.0f),
        //     Vec3(-1.0f,  1.0f,  -1.0f)
        // };

        // std::vector<Vec3> vertices = {
        //     Vec3(0.5f, 0.5f, 0.0f),
        //     Vec3(0.5f, -0.5f, 0.0f),
        //     Vec3(-0.5f, -0.5f, 0.0f),
        //     Vec3(-0.5f, 0.5f, 0.0f)
        // };

        // std::vector<unsigned int> indices =
        // {
        //     // front
        //     0, 1, 2,
        //     2, 3, 0, 
        //     // right
        //     1, 5, 6,
        //     6, 2, 1,
        //     // back
        //     7, 6, 5,
        //     5, 4, 7,
        //     // left
        //     4, 0, 3,
        //     3, 7, 4,
        //     // bottom
        //     4, 5, 1,
        //     1, 0, 4,
        //     // top
        //     3, 2, 6,
        //     6, 7, 3
        // };

        std::vector<std::string> faces =
        {
            "assets/textures/Skybox/right.png",
            "assets/textures/Skybox/left.png",
            "assets/textures/Skybox/top.png",
            "assets/textures/Skybox/bottom.png",
            "assets/textures/Skybox/front.png",
            "assets/textures/Skybox/back.png",
        };

        auto mesh = GLMesh(GLVertexArray(vertices));

        model.AddMesh(mesh, GLMaterial(shader));

        model.Get().AddSkybox(faces);
    };

    void GLRenderer::Update(const Controller::Time& time) 
    {
        glClearColor(0.52941f, 0.80784f, 0.92157f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDepthFunc(GL_LEQUAL);

        model.Draw(GL_SHADER);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)1280 / (float) 720, 0.1f, 100.0f);
        glm::mat4 view = glm::mat4(glm::mat3(glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f))));

        model.Get().SetMat4("projection", projection);
        model.Get().SetMat4("view", view);

        model.Get().SetFloat4("rgba", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        model.Get().SetBool("enableTexture", true);

        model.Draw(GL_TEXTURE_CUBE_MAP);

        glDepthFunc(GL_LESS);

        //glm::mat4 model_matrix = glm::mat4(1.0f);

        //model_matrix = glm::translate(model_matrix, glm::vec3(0.0f, 0.0f, -5.0f));

        //model_matrix = glm::scale(model_matrix, glm::vec3(1.0f));

        //model_matrix = glm::rotate(model_matrix, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        //model_matrix = glm::rotate(model_matrix, 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        //model_matrix = glm::rotate(model_matrix, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));

        //model.Get().SetMat4("model", model_matrix);
        //model.Get().SetMat3("normalMatrix", glm::transpose(glm::inverse(model_matrix)));

        //model.Draw(GL_TEXTURE_CUBE_MAP);
    };
}