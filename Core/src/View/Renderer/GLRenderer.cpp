#include "GLRenderer.hpp"

#include <glad/glad.h>

#include <Controller/Logger.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Model/Components.hpp"
#include "Model/gl/GLShader.hpp"

namespace Vakol::View {
    GLRenderer::GLRenderer(const std::shared_ptr<Window> window) : Renderer(window) {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);
    };

    void GLRenderer::Draw(const Controller::Time& time, const Controller::Camera& camera, const Model::Components::Drawable& drawable) const 
    {
        glClearColor(0.52941f, 0.80784f, 0.92157f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model_matrix = glm::mat4(1.0f);

        model_matrix = glm::translate(model_matrix, glm::vec3(0.0f));

        model_matrix = glm::scale(model_matrix, glm::vec3(1.0f));

        model_matrix = glm::rotate(model_matrix, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        model_matrix = glm::rotate(model_matrix, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        model_matrix = glm::rotate(model_matrix, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));

        for (auto mesh : drawable.model_ptr->meshes()) {
            mesh.material()->Bind();
            mesh.material()->SetMat4("_PVM", camera.GetMatrix(_PV_MATRIX) * model_matrix);

            mesh.vao()->Draw();
        }
    }
}