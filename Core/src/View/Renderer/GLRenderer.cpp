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

    // void Draw(const Controller::Time& time, const std::shared_ptr<Model::Components::Drawable> drawable) {}

    void GLRenderer::Update() {
        glClearColor(0.52941f, 0.80784f, 0.92157f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    };
}  // namespace Vakol::View