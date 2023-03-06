#include "RendererFactory.hpp"

#include "GLRenderer.hpp"

namespace Vakol::View {
    std::shared_ptr<Renderer> CreateRenderer(const std::string& type, const std::shared_ptr<Window> window) {
        if (type == "gl") {
            return std::make_shared<View::GLRenderer>(window);
        }
        return nullptr;
    }
}  // namespace Vakol::View