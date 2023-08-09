#include "include/RendererFactory.hpp"

#include "Logger/include/Logger.hpp"
#include "include/GLRenderer.hpp"

namespace Vakol
{
    std::shared_ptr<Renderer> CreateRenderer(const std::string& type, const std::shared_ptr<Window>& window)
    {
        if (type == "gl")
            return std::make_shared<GLRenderer>(window);
        else
        {
            VK_CRITICAL("Renderer type {0} is not supported", type);
            return nullptr;
        }
    }
} // namespace Vakol