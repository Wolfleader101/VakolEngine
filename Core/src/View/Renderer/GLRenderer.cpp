#include "GLRenderer.hpp"

#include <glad/glad.h>

#include <vector>
#include <memory>

#include <Controller/Logger.hpp>

#include "Model/Components.hpp"
#include "Model/gl/GLUniformBuffer.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
const glm::vec4 VAKOL_DARK = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);

namespace Vakol::View {
    GLRenderer::GLRenderer(const std::shared_ptr<Window> window) : Renderer(window) 
    {
        glEnable(GL_DEPTH_TEST);
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    };

    void GLRenderer::AddUniform(const int size, const int binding)
	{
		this->m_uniforms.push_back(std::make_shared<GLUniformBuffer>(size, binding));
	}

	void GLRenderer::SetUniformData(const int index, const int offset, const int size, const void* data) const
	{
		if (this->m_uniforms[index]) this->m_uniforms[index]->SetData(offset, size, data);
		else VK_ERROR("Uniform buffer at index {0} does not exist!", index);
	}

    void GLRenderer::ClearColor(const glm::vec4& color) const { glClearColor(color.r, color.g, color.b, color.a); }
    void GLRenderer::ClearColor(const float r, const float g, const float b, const float a) const { glClearColor(r,g,b,a); }

    void GLRenderer::ClearBuffer(const unsigned int buffer_bit) const { glClear(buffer_bit); }

    void GLRenderer::Draw(const Controller::Time& time, const Controller::Camera& camera, const Model::Components::Transform trans, const Model::Components::Drawable& drawable) const 
    {
        SetUniformData(0, 0, sizeof(glm::mat4), glm::value_ptr(camera.GetMatrix(PV_MATRIX)));

        glm::mat4 model_matrix = glm::mat4(1.0f);

        model_matrix = glm::translate(model_matrix, trans.pos);

        model_matrix = glm::scale(model_matrix, trans.scale);

        model_matrix = glm::rotate(model_matrix, trans.rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
        model_matrix = glm::rotate(model_matrix, trans.rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
        model_matrix = glm::rotate(model_matrix, trans.rot.z, glm::vec3(0.0f, 0.0f, 1.0f));

        drawable.model_ptr->shader()->SetMat4("MODEL_MATRIX", model_matrix);

        for (int i = 0; i < drawable.model_ptr->mesh_count(); ++i)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 2 * (i + 1));

            drawable.model_ptr->meshes().at(i).vao()->DrawElementsInstanced(1000);
        }
    }

    void GLRenderer::Update() const
    {
        ClearColor(VAKOL_CLASSIC);
        ClearBuffer(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}  // namespace Vakol::View