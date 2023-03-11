#include "GLRenderer.hpp"

#include <glad/glad.h>

#include <Model/Math/Vector3.hpp>
#include <Model/Math/Vector2.hpp>

#include <vector>


using Vakol::Model::Math::Vec3;
using Vakol::Model::Math::Vec2;

namespace Vakol::View 
{
    GLRenderer::GLRenderer(const std::shared_ptr<Window> window) : Renderer(window) 
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);

        // // simple cube
        // std::vector<Vertex3> cube_vertices = 
        // {
        //     {
        //         // front
        //         {{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        //         {{ 1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        //         {{ 1.0f,  1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        //         {{-1.0f,  1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
        //         // top
        //         {{-1.0f,  1.0f,  1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        //         {{ 1.0f,  1.0f,  1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        //         {{ 1.0f,  1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        //         {{-1.0f,  1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
        //         // back
        //         {{ 1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
        //         {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
        //         {{-1.0f,  1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
        //         {{ 1.0f,  1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
        //         // bottom
        //         {{-1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
        //         {{ 1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
        //         {{ 1.0f, -1.0f,  1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
        //         {{-1.0f, -1.0f,  1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
        //         // left
        //         {{-1.0f, -1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        //         {{-1.0f, -1.0f,  1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        //         {{-1.0f,  1.0f,  1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        //         {{-1.0f,  1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
        //         // right
        //         {{1.0f, -1.0f,  1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        //         {{1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        //         {{1.0f,  1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        //         {{1.0f,  1.0f,  1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}
        //     }
        // };

        // std::vector<unsigned int> cube_indices =
        // {
        //     // front
        //     0,  1,  2,
        //     2,  3,  0,
        //     // top
        //     4,  5,  6,
        //     6,  7,  4,
        //     // back
        //     8,  9, 10,
        //     10, 11,  8,
        //     // bottom
        //     12, 13, 14,
        //     14, 15, 12,
        //     // left
        //     16, 17, 18,
        //     18, 19, 16,
        //     // right
        //     20, 21, 22,
        //     22, 23, 20,
        // };


        // // different texture files that make up the cubemap texture of a skybox
        // std::vector<std::string> skybox_faces =
        // {
        //     "assets/textures/Skybox/right.png",
        //     "assets/textures/Skybox/left.png",
        //     "assets/textures/Skybox/top.png",
        //     "assets/textures/Skybox/bottom.png",
        //     "assets/textures/Skybox/front.png",
        //     "assets/textures/Skybox/back.png",
        // };
    

    };

    void GLRenderer::Update(const Controller::Time& time) 
    {
        //glClearColor(0.52941f, 0.80784f, 0.92157f, 1.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //model.Draw(GL_SHADER);

        glm::mat4 proj_mat = glm::perspective(glm::radians(45.0f), (float) 1280 / (float) 720, 0.01f, 100.0f);
        glm::mat4 view_mat = glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        //model.Get().SetMat4("projection", proj_mat);
        //model.Get().SetMat4("view", view_mat);

        glm::mat4 mdl_mat = glm::mat4(1.0f);

        mdl_mat = glm::translate(mdl_mat, glm::vec3(0.0f, 0.0f, -5.0f));

        mdl_mat = glm::scale(mdl_mat, glm::vec3(1.0f));

        mdl_mat = glm::rotate(mdl_mat, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        mdl_mat = glm::rotate(mdl_mat, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        mdl_mat = glm::rotate(mdl_mat, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));

        // model.Get().SetMat4("model", mdl_mat);

        // /************* Vert Shader *********/ 
        // model.Get().SetMat3("normal_matrix", glm::mat3(glm::transpose(glm::inverse(mdl_mat))));
        // /***********************************/


        // /************** Frag Shader ***********************/
        // model.Get().SetFloat("material.shininess", 32.0f);

        // model.Get().SetVec3("light.position", glm::vec3(0.0f, 0.5f, -2.0f));
        // //model.Get().SetVec3("light.color", glm::vec3(1.0f, 1.0f, 1.0f)); // not used as the color is determined by the texture

        // model.Get().SetVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        // model.Get().SetVec3("light.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
        // model.Get().SetVec3("light.specular", glm::vec3(0.6f, 0.6f, 0.6f));
        
        // model.Get().SetVec3("viewPos", glm::vec3(0.0f));

        // /***************************************************/

        // model.Draw(GL_TEXTURE_2D);
    };
}