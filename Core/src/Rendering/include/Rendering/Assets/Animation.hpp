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
             * \brief the position
             */
            Math::Vec3 position;

            /**
             * \brief the timestamp at a particular time
             */
            double timestamp;
        };

        struct Rotation
        {
            /**
             * \brief the rotation
             */
            Math::Quat rotation;

            /**
             * \brief the timestamp at a particular time
             */
            double timestamp;
        };

        struct Scale
        {
            /**
             * \brief the scale
             */
            Math::Vec3 scale;

            /**
             * \brief the timestamp at a particular time
             */
            double timestamp;
        };

        /**
         * \brief the position keyframes of a channel.
         */
        std::vector<Position> positions;
        /**
         * \brief the rotation keyframes of a channel
         */
        std::vector<Rotation> rotations;
        /**
         * \brief the scales keyframes of a channel
         */
        std::vector<Scale> scales;
    };

    struct Animation
    {
        /**
         * \brief the name of the animation.
         */
        const char* name = nullptr;

        /**
         * \brief the duration of an animation (multiply by ticks per second)
         */
        double duration = 0.0;

        /**
         * \brief the ticks per second of an animation
         */
        double ticksPerSecond = 0.0;

        /**
         * \brief the channels of an animation.
         */
        std::vector<Channel> channels;
    };
} // namespace Vakol::Rendering::Assets
