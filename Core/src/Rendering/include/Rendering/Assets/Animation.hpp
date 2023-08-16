#pragma once

#include "Math/Math.hpp"

#include <vector>

namespace Vakol::Rendering::Assets
{
    struct Channel
    {
        const char* name = nullptr;

        struct Position
        {
            Math::Vec3 position;
            double timestamp;
        };

        struct Rotation
        {
            Math::Quaternion rotation;
            double timestamp;
        };

        struct Scale
        {
            Math::Vec3 scale;
            double timestamp;
        };

        std::vector<Position> positions;
        std::vector<Rotation> rotations;
        std::vector<Scale> scales;
    };

    struct Animation
    {
        const char* name = nullptr;

        double duration = 0.0;
        double ticksPerSecond = 0.0;

        std::vector<Channel> channels;
    };
} // namespace Vakol::Rendering::Assets