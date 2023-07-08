#pragma once

#include <View/Window/Window.hpp>

namespace Vakol {
    namespace Controller {
        /**
         * @struct Time
         * @brief time data used everywhere
         */
        struct Time {
            Time(){};
            /**
             * @brief current time
             */
            double curTime = 0.0f;

            /**
             * @brief delta time
             */
            double deltaTime = 0.0f;

            /**
             * @brief the previous frames time
             *
             */
            double prevTime = 0.0f;

            /**
             * @brief  the number of frames per second
             *
             */
            int fps = 0;

            double tickRate = 1.0f / 60.0f;

            double accumulator = 0.0f;

            /**
             * @brief used to update the time struct
             */
            void Update() {
                curTime = glfwGetTime();
                frameCount++;

                if (curTime - prevFPSTime >= 1.0f) {
                    fps = frameCount;
                    frameCount = 0;
                    prevFPSTime = curTime;
                }

                deltaTime = curTime - prevTime;

                prevTime = curTime;
            }

           private:
            /**
             * @brief internal frame count
             *
             */
            int frameCount = 0;
            double prevFPSTime = 0.0f;
        };
    }  // namespace Controller
}  // namespace Vakol
