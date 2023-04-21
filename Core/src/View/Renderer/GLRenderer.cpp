#include "GLRenderer.hpp"

#include <glad/glad.h>

#include <Controller/Logger.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <vector>

#include "Model/Components.hpp"
#include "Model/gl/GLUniformBuffer.hpp"

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

// void OpenGLMessageCallback(
// 		unsigned source,
// 		unsigned type,
// 		unsigned id,
// 		unsigned severity,
// 		int length,
// 		const char* message,
// 		const void* userParam)
// {
//     switch (severity)
//     {
//         case GL_DEBUG_SEVERITY_HIGH:         VK_CRITICAL(message); return;
//         case GL_DEBUG_SEVERITY_MEDIUM:       VK_ERROR(message); return;
//         case GL_DEBUG_SEVERITY_LOW:          VK_WARN(message); return;
//         case GL_DEBUG_SEVERITY_NOTIFICATION: VK_TRACE(message); return;
//     }
// }

const glm::vec4 VAKOL_CLASSIC = glm::vec4(0.52941f, 0.80784f, 0.92157f, 1.0f);

const glm::vec4 VAKOL_FOGGY = glm::vec4(0.4f, 0.4f, 0.4f, 1.0);
const glm::vec4 VAKOL_FOGGY_2 = glm::vec4(0.8f, 0.8f, 0.8f, 0.0);

const glm::vec4 VAKOL_DARK = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);

const float light_constant = 1.0f;
const float light_linear = 0.09f;
const float light_quadratic = 0.032f;

const float light_cut_off = glm::cos(glm::radians(7.5f));
const float light_outer_cut_off = glm::cos(glm::radians(12.5f));

namespace Vakol::View {
    GLRenderer::GLRenderer(const std::shared_ptr<Window> window) : Renderer(window) {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        AddUniform(2 * sizeof(glm::mat4), 1);
        // AddUniform(5 * sizeof(float), 2);
        AddUniform(sizeof(float), 3);

        // SetUniformData(1, 0, sizeof(float), &light_constant);
        // SetUniformData(1, 1 * sizeof(float), sizeof(float), &light_linear);
        // SetUniformData(1, 2 * sizeof(float), sizeof(float), &light_quadratic);

        // SetUniformData(1, 3 * sizeof(float), sizeof(float), &light_cut_off);
        // SetUniformData(1, 4 * sizeof(float), sizeof(float), &light_outer_cut_off);
    };

    void GLRenderer::AddUniform(const int size, const int binding) {
        this->m_uniforms.push_back(std::make_shared<GLUniformBuffer>(size, binding));
    }

    void GLRenderer::SetUniformData(const int index, const int offset, const int size, const void* data) const {
        if (this->m_uniforms[index])
            this->m_uniforms[index]->SetData(offset, size, data);
        else
            VK_ERROR("Uniform buffer at index {0} does not exist!", index);
    }

    void GLRenderer::ClearColor(const glm::vec4& color) const { glClearColor(color.r, color.g, color.b, color.a); }
    void GLRenderer::ClearColor(const float r, const float g, const float b, const float a) const {
        glClearColor(r, g, b, a);
    }

    void GLRenderer::ClearBuffer(const unsigned int buffer_bit) const { glClear(buffer_bit); }

    void GLRenderer::Draw(const Controller::Time& time, const Controller::Camera& camera,
                          const Model::Components::Transform trans, const Model::Components::Drawable& drawable) const {
        drawable.model_ptr->GetShader()->Bind();
        SetUniformData(0, 0, sizeof(glm::mat4), glm::value_ptr(camera.GetMatrix(PV_MATRIX)));
        SetUniformData(0, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera.GetMatrix(VIEW_MATRIX)));

        SetUniformData(1, 0, sizeof(float), &time.curTime);

        glm::mat4 model_matrix = glm::mat4(1.0f);

        model_matrix = glm::translate(model_matrix, trans.pos);

        model_matrix = glm::rotate(model_matrix, trans.rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
        model_matrix = glm::rotate(model_matrix, trans.rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
        model_matrix = glm::rotate(model_matrix, trans.rot.z, glm::vec3(0.0f, 0.0f, 1.0f));

        model_matrix = glm::scale(model_matrix, trans.scale);

        drawable.model_ptr->GetShader()->SetMat4("MODEL_MATRIX", model_matrix);
        // drawable.model_ptr->GetShader()->SetMat3("NORMAL_MATRIX",
        // glm::transpose(glm::inverse(glm::mat3(model_matrix))));

        // drawable.model_ptr->GetShader()->SetVec3v("VIEW_POS", camera.GetPos());

        // drawable.model_ptr->GetShader()->SetVec3v("light.position", camera.GetPos());
        // drawable.model_ptr->GetShader()->SetVec3v("light.direction", camera.GetForward());

        for (int i = 0; i < drawable.model_ptr->GetMeshCount(); ++i) {
            auto mesh = drawable.model_ptr->GetMeshes().at(i);

            switch (mesh.GetVertexArray()->GetDrawMode()) {
                case ARRAYS:
                    mesh.GetVertexArray()->DrawArrays();
                    break;
                case ELEMENTS:
                    mesh.GetVertexArray()->DrawElements();
                    break;
                case ARRAYS_INSTANCED:
                    mesh.GetVertexArray()->DrawArraysInstanced(1000);
                    break;
                case ELEMENTS_INSTANCED:
                    mesh.GetVertexArray()->DrawElementsInstanced(1000);
                    break;
                case TRIANGLE_STRIPS:
                    mesh.GetVertexArray()->DrawTriangleStrips();
                    break;
                case QUAD_PATCHES:
                    mesh.GetVertexArray()->DrawQuadPatches();
                    break;
                default:
                    break;
            }
        }
        drawable.model_ptr->GetShader()->Unbind();
    }

    void GLRenderer::Update() const {
        ClearColor(VAKOL_CLASSIC);
        ClearBuffer(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}  // namespace Vakol::View