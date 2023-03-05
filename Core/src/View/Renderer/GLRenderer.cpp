#include "GLRenderer.hpp"

#include <glad/glad.h>

namespace Vakol::View {
GLRenderer::GLRenderer(const std::shared_ptr<Window> window) : Renderer(window) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
};
void GLRenderer::Update(const Controller::Time& time) {
    glClearColor(0.52941f, 0.80784f, 0.92157f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
};
}  // namespace Vakol::View