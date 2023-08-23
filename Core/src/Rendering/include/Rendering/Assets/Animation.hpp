#pragma once

#include "Math/Math.hpp"

#include <vector>

namespace Vakol::Rendering::Assets
{
    struct Channel
    {
        /**
         * \brief 
         */
        const char* name = nullptr;

        struct Position
        {
            /**
             * \brief 
             */
            Math::Vec3 position;

            /**
             * \brief 
             */
            double timestamp;
        };

        struct Rotation
        {
            /**
             * \brief 
             */
            Math::Quat rotation;

            /**
             * \brief 
             */
            double timestamp;
        };

        /**
         * \brief 
         */
        struct Scale
        {
            /**
             * \brief 
             */
            Math::Vec3 scale;

            /**
             * \brief 
             */
            double timestamp;
        };

        /**
         * \brief 
         */
        std::vector<Position> positions;
        /**
         * \brief 
         */
        std::vector<Rotation> rotations;
        /**
         * \brief 
         */
        std::vector<Scale> scales;
    };

    struct Animation
    {
        /**
         * \brief 
         */
        const char* name = nullptr;

        /**
         * \brief 
         */
        double duration = 0.0;

        /**
         * \brief 
         */
        double ticksPerSecond = 0.0;

        /**
         * \brief 
         */
        std::vector<Channel> channels;
    };
} // namespace Vakol::Rendering::Assets