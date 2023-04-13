#include "GLRenderer.hpp"

#include <glad/glad.h>

#include <Controller/Logger.hpp>
#include <glm/common.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

namespace Vakol::View {
    GLRenderer::GLRenderer(const std::shared_ptr<Window> window) : Renderer(window) {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);
    };

    void GLRenderer::Draw(const Controller::Time& time, const Model::Components::Drawable& drawable) const
    {
        glClearColor(0.52941f, 0.80784f, 0.92157f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // This will be neater once we get a Camera put in
        glm::mat4 proj_mat = glm::perspective(glm::radians(45.0f), (float)1280 / (float)720, 0.01f, 100.0f);
        glm::mat4 view_mat = glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        // mdl_mat as in model matrix
        glm::mat4 mdl_mat = glm::mat4(1.0f);

        mdl_mat = glm::translate(mdl_mat, glm::vec3(0.0f, 0.0f, -5.0f));

        mdl_mat = glm::scale(mdl_mat, glm::vec3(1.0f));

        // I think I read somewhere that you could reduce this down to one line
        mdl_mat = glm::rotate(mdl_mat, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        mdl_mat = glm::rotate(mdl_mat, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        mdl_mat = glm::rotate(mdl_mat, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    }
}  // namespace Vakol::View