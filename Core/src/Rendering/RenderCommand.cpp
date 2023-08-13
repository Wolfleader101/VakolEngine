#include "RenderCommand.hpp"

#include <crossguid/guid.hpp>

namespace Vakol::Rendering
{
    std::string GenerateID()
    {
        return xg::newGuid().str();
    }

}
