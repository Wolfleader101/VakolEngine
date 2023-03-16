#include "GLRenderer.hpp"

#include <glad/glad.h>

#include <Controller/MeshFilter.hpp>
#include <Controller/MeshRenderer.hpp>

#include <Controller/Logger.hpp>

#include <Model/Math/Vector3.hpp>
#include <Model/Math/Vector2.hpp>

#include <vector>

using Vakol::Model::Math::Vec3;
using Vakol::Model::Math::Vec2;

using Vakol::Controller::MeshFilter;
using Vakol::Controller::MeshRenderer;

using Vakol::Model::DRAW_MODE_INDEXED;
using Vakol::Model::DRAW_MODE_NON_INDEXED;

namespace Vakol::View 
{
    GLRenderer::GLRenderer(const std::shared_ptr<Window> window) : Renderer(window) 
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);
    
        MeshFilter::LoadMesh("assets/models/cube.obj");
        MeshRenderer::LoadMaterials();
    };

    void GLRenderer::Update(const Controller::Time& time) 
    {
        glClearColor(0.52941f, 0.80784f, 0.92157f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        MeshRenderer::Draw(GL_SHADER);

        // This will be neater once we get a Camera put in
        glm::mat4 proj_mat = glm::perspective(glm::radians(45.0f), (float) 1280 / (float) 720, 0.01f, 100.0f);
        glm::mat4 view_mat = glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        // unsure if GetMaterial() would be a better name
        MeshRenderer::Get(0).SetMat4("projection", proj_mat);
        MeshRenderer::Get(0).SetMat4("view", view_mat);

        // mdl_mat as in model matrix
        glm::mat4 mdl_mat = glm::mat4(1.0f);

        mdl_mat = glm::translate(mdl_mat, glm::vec3(0.0f, 0.0f, -5.0f));

        mdl_mat = glm::scale(mdl_mat, glm::vec3(1.0f));

        // I think I read somewhere that you could reduce this down to one line
        mdl_mat = glm::rotate(mdl_mat, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        mdl_mat = glm::rotate(mdl_mat, 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        mdl_mat = glm::rotate(mdl_mat, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));

        MeshRenderer::Get(0).SetMat4("model", mdl_mat);

        // /************* Vert Shader *********/ 
        MeshRenderer::Get(0).SetMat3("normal_matrix", glm::mat3(glm::transpose(glm::inverse(mdl_mat))));
        // /***********************************/


        // /************** Frag Shader ***********************/

        MeshRenderer::Get(0).SetVec3("light.position", glm::vec3(0.0f, 0.5f, -2.0f));
        
        MeshRenderer::Get(0).SetVec3("viewPos", glm::vec3(0.0f));

        // // /***************************************************/

        MeshRenderer::Draw(GL_TEXTURE_2D);
        MeshFilter::Draw(DRAW_MODE_INDEXED);
    };
}