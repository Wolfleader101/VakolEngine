
#include "include/Time.hpp"
#include "Window/include/Window.hpp"

namespace Vakol
{

    Time::Time()
    {
    }

    void Time::Update()
    {
        curTime = glfwGetTime();
        frameCount++;

        if (curTime - prevFPSTime >= 1.0)
        {
            fps = frameCount;
            frameCount = 0;
            prevFPSTime = curTime;
        }

        deltaTime = curTime - prevTime;

        prevTime = curTime;
    }

} // namespace Vakol
