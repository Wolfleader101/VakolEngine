#pragma once

#include <View/Window/Window.hpp>

namespace Vakol {
    namespace Controller {
        /**
         * @struct Time
         * @brief time data used everywhere
         */
        struct Time {

            Time() {};
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
             * @brief used to update the time struct
             */
            void Update() {
                curTime = glfwGetTime();
                if (prevTime == 0.0f)
                    deltaTime = 0.0f;
                else
                    deltaTime = curTime - prevTime;
                prevTime = curTime;
            }
        };
    }
}  // namespace Vakol::Controller
