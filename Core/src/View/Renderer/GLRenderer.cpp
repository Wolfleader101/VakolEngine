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

#include <Model/Components.hpp>

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

constexpr glm::vec4 VAKOL_CLASSIC = glm::vec4(0.52941f, 0.80784f, 0.92157f, 1.0f);

constexpr glm::vec4 VAKOL_FOGGY = glm::vec4(0.4f, 0.4f, 0.4f, 1.0);
constexpr glm::vec4 VAKOL_FOGGY_2 = glm::vec4(0.8f, 0.8f, 0.8f, 0.0);

constexpr glm::vec4 VAKOL_DARK = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);

constexpr float LIGHT_CONSTANT = 1.0f;
constexpr float LIGHT_LINEAR = 0.09f;
constexpr float LIGHT_QUADRATIC = 0.032f;

const float LIGHT_CUT_OFF = glm::cos(glm::radians(7.5f));
const float LIGHT_OUTER_CUT_OFF = glm::cos(glm::radians(12.5f));

namespace Vakol::View 
{
    GLRenderer::GLRenderer(const std::shared_ptr<Window>& window) : Renderer(window) 
    {
        glEnable(GL_DEPTH_TEST);
        //glEnable(GL_CULL_FACE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // this corresponds to the uniform buffer in each shader that has one.
        // layout (std140, binding = 1) uniform <name>
        // std140 - memory layout, binding - index, uniform (typeof buffer)
        AddBuffer(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), 1, GL_STATIC_DRAW);
        // add a uniform buffer which size is that of a 4x4 matrix with a binding index of 1
    };

    void GLRenderer::AddBuffer(const unsigned int type, const int size, const int binding, const void* data, const unsigned int usage)
    {
        buffers.push_back(std::make_shared<Buffer>(type, size, binding, data, usage));
    }

    void GLRenderer::AddBuffer(const unsigned int type, const int size, const int binding, const unsigned int usage)
    {
        buffers.push_back(std::make_shared<Buffer>(type, size, binding, usage));
    }

    void GLRenderer::SetBufferSubData(const int index, const int offset, const int size, const void* data) const
    {
        buffers.at(index)->SetSubData(offset, size, data);
    }

    void GLRenderer::ClearColor(const glm::vec4& color)
    { glClearColor(color.r, color.g, color.b, color.a); }

    void GLRenderer::ClearColor(const float r, const float g, const float b, const float a)
    { glClearColor(r, g, b, a); }

    void GLRenderer::ClearBuffer(const unsigned int buffer_bit)
    { glClear(buffer_bit); }

    void GLRenderer::Draw([[maybe_unused]] const Controller::Time& time, const Controller::Camera& camera, const Model::Components::Transform trans, const Model::Components::Drawable& drawable) const 
    {
        VK_ASSERT(drawable.model_ptr, "\n\nModel ptr is nullptr");

        const auto& shader = drawable.model_ptr->GetShader();

        VK_ASSERT(shader, "\n\nShader is nullptr");

        shader->Bind();

        // at index 0, with an offset of 0 (since PV_MATRIX is the only element in the buffer), with a size of a 4x4 matrix, set PV_MATRIX
        SetBufferSubData(0, 0, sizeof(glm::mat4), glm::value_ptr(camera.GetMatrix(PV_MATRIX)));
        SetBufferSubData(0, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera.GetMatrix(VIEW_MATRIX)));

        auto model_matrix = glm::mat4(1.0f); // start off with an identity matrix

        model_matrix = glm::translate(model_matrix, trans.pos);

        model_matrix = glm::scale(model_matrix, trans.scale);

        model_matrix = glm::rotate(model_matrix, glm::radians(trans.rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model_matrix = glm::rotate(model_matrix, glm::radians(trans.rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model_matrix = glm::rotate(model_matrix, glm::radians(trans.rot.z), glm::vec3(0.0f, 0.0f, 1.0f));

        SetBufferSubData(0, 2 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(model_matrix));

        for (int i = 0; i < drawable.model_ptr->nMeshes(); ++i) 
        {
            const auto& mesh = drawable.model_ptr->meshes().at(i);
            const auto& material = mesh.GetMaterial();

            for (int j = 0; j < material->GetTextureCount(); ++j)
            {
                glActiveTexture(GL_TEXTURE0 + j);
                glBindTexture(GL_TEXTURE_2D, material->GetTexture(j).GetID());
            }

            mesh.Draw();
        }

        shader->Unbind();
    }

    void GLRenderer::Update() const 
    {
        ClearColor(VAKOL_CLASSIC);
        ClearBuffer(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}