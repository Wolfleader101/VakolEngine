#include "RendererFactory.hpp"

#include "Controller/Logger.hpp"
#include "GLRenderer.hpp"

namespace Vakol::View
{
    std::shared_ptr<Renderer> CreateRenderer(const std::string& type, const std::shared_ptr<Window>& window)
    {
        if (type == "gl")
            return std::make_shared<View::GLRenderer>(window);
        else
        {
            VK_CRITICAL("Renderer type {0} is not supported", type);
            return nullptr;
        }
    }
} // namespace Vakol::View