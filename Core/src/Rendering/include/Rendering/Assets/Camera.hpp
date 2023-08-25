#pragma once

#include "Math/Math.hpp"

namespace Vakol::Rendering::Assets
{
    // This will likely not be used.

    Math::Mat4 PROJECTION_MATRIX = Math::Mat4(1.0f);
    Math::Mat4 VIEW_MATRIX = Math::Mat4(1.0f);

    float aspect = 0.0f;
    float fov = 0.0f;
    float near = 0.0f;
    float far = 0.0f;
} // namespace Vakol::Rendering::Assets