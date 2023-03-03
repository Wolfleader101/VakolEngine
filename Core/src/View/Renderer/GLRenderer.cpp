#include "GLRenderer.hpp"

#include <glad/glad.h>

namespace Vakol::View {
GLRenderer::GLRenderer(const std::shared_ptr<Window> window) : Renderer(window){};
void GLRenderer::Update(const Controller::Time& time){};
}  // namespace Vakol::View