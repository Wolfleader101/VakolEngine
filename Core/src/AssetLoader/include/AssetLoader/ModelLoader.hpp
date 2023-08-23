#pragma once

#include "Rendering/Assets/Model.hpp"

namespace Vakol
{
    /**
     * \brief
     * \param path the path of the model
     * \param scale the global scale of the model
     * \param success does the model successfully load?
     * \return
     */
    Rendering::Assets::Model ImportModel(const char* path, float scale, bool& success);
} // namespace Vakol
