#pragma once

namespace Vakol
{
    /**
     * @struct Time
     * @brief time data used everywhere
     */
    struct Time
    {
        Time();

        /**
         * @brief current time
         */
        double curTime = 0.0;

        /**
         * @brief delta time
         */
        double deltaTime = 0.0;

        /**
         * @brief the previous frames time
         *
         */
        double prevTime = 0.0;

        /**
         * @brief  the number of frames per second
         *
         */
        int fps;

        double tickRate = 1.0 / 60.0;

        double accumulator = 0.0;

        /**
         * @brief used to update the time struct
         */
        void Update();

      private:
        /**
         * @brief internal frame count
         *
         */
        int frameCount = 0;
        double prevFPSTime = 0.0;
    };
} // namespace Vakol
