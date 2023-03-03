#include <View/Renderer/Renderer.hpp>

namespace Vakol::View {
GLRenderer::GLRenderer(const std::shared_ptr<Window> window); : Renderer(window){};
void GLRenderer::Update(const Controller::Time& time) override{};
}  // namespace Vakol::View