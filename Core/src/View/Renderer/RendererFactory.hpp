#include <memory>
#include <string>

#include "Renderer.hpp"

namespace Vakol::View {
    std::shared_ptr<Renderer> CreateRenderer(const std::string& type, const std::shared_ptr<Window> window);
}