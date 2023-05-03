#include "GLRenderer.hpp"

#include <glad/glad.h>

#include <Controller/Logger.hpp>

#pragma warning(push)
#pragma warning(disable:4201)
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma warning(pop)

#include <memory>
#include <vector>

#include "Model/Components.hpp"

/*
Distance	Constant	Linear	Quadratic
7	        1.0	        0.7	    1.8
13	        1.0	        0.35	0.44
20	        1.0	        0.22	0.20
32	        1.0	        0.14	0.07
50	        1.0	        0.09	0.032
65	        1.0	        0.07	0.017
100	        1.0	        0.045	0.0075
160	        1.0	        0.027	0.0028
200	        1.0	        0.022	0.0019
325	        1.0	        0.014	0.0007
600	        1.0	        0.007	0.0002
3250	    1.0	        0.0014	0.000007
*/

const glm::vec4 VAKOL_CLASSIC = glm::vec4(0.52941f, 0.80784f, 0.92157f, 1.0f);

const glm::vec4 VAKOL_FOGGY = glm::vec4(0.4f, 0.4f, 0.4f, 1.0);
const glm::vec4 VAKOL_FOGGY_2 = glm::vec4(0.8f, 0.8f, 0.8f, 0.0);

const glm::vec4 VAKOL_DARK = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);

const float light_constant = 1.0f;
const float light_linear = 0.09f;
const float light_quadratic = 0.032f;

const float light_cut_off = glm::cos(glm::radians(7.5f));
const float light_outer_cut_off = glm::cos(glm::radians(12.5f));

namespace Vakol::View 
{
    GLRenderer::GLRenderer(const std::shared_ptr<Window> window) : Renderer(window) 
    {
        glEnable(GL_DEPTH_TEST);
        //glEnable(GL_CULL_FACE);

        //glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    };

    void GLRenderer::ClearColor(const glm::vec4& color) const { glClearColor(color.r, color.g, color.b, color.a); }
    void GLRenderer::ClearColor(const float r, const float g, const float b, const float a) const { glClearColor(r, g, b, a); }

    void GLRenderer::ClearBuffer(const unsigned int buffer_bit) const { glClear(buffer_bit); }

    void GLRenderer::Draw(const Controller::Time& time, const Controller::Camera& camera, const Model::Components::Transform trans, const Model::Components::Drawable& drawable) const 
    {
        drawable.model_ptr->GetShader()->Bind();

        glm::mat4 model_matrix = glm::mat4(1.0f);

        model_matrix = glm::translate(model_matrix, trans.pos);

        model_matrix = glm::rotate(model_matrix, trans.rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
        model_matrix = glm::rotate(model_matrix, trans.rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
        model_matrix = glm::rotate(model_matrix, trans.rot.z, glm::vec3(0.0f, 0.0f, 1.0f));

        model_matrix = glm::scale(model_matrix, trans.scale);

        drawable.model_ptr->GetShader()->SetMat4("PV_MATRIX", camera.GetMatrix(PV_MATRIX));
        drawable.model_ptr->GetShader()->SetMat4("MODEL_MATRIX", model_matrix);
        // drawable.model_ptr->GetShader()->SetMat3("NORMAL_MATRIX",
        // glm::transpose(glm::inverse(glm::mat3(model_matrix))));

        // drawable.model_ptr->GetShader()->SetVec3v("VIEW_POS", camera.GetPos());

        // drawable.model_ptr->GetShader()->SetVec3v("light.position", camera.GetPos());
        // drawable.model_ptr->GetShader()->SetVec3v("light.direction", camera.GetForward());

        for (int i = 0; i < drawable.model_ptr->GetMeshCount(); ++i) 
        {
            auto mesh = drawable.model_ptr->GetMeshes().at(i);
        }

        //drawable.model_ptr->GetShader()->Unbind();
    }

    void GLRenderer::Update() const 
    {
        ClearColor(VAKOL_CLASSIC);
        ClearBuffer(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}  // namespace Vakol::View