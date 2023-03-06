#pragma once

#include "Math.hpp"

namespace Vakol::Model::Math {
    struct Quaternion {
        Quaternion() = default;

        glm::quat quat;
    };
    typedef Quaternion Quat;
}  // namespace Vakol::Model::Math
