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
    bool ImportModel(Rendering::Assets::Model& model, const char* path, const float scale);
} // namespace Vakol
