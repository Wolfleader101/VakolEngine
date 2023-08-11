#include <memory>
#include <string>

#include "Renderer.hpp"

namespace Vakol
{
    /**
     * @brief Factory function to create Renderer objects
     * @param type Type of renderer to be created
     * @param window Shared pointer to a Window object
     * @return Shared pointer to a newly created Renderer object
     */
    std::shared_ptr<Renderer> CreateRenderer(const std::string& type, const std::shared_ptr<Window>& window);
} // namespace Vakol