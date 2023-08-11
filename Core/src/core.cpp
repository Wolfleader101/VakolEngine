#include "../include/Vakol/core.hpp"

#include "Application/include/Application.hpp"
#include "Utils/include/Singleton.hpp"
#include <iostream>

namespace Vakol
{
    void Run()
    {
        Logger::Init();

        Singleton<Application>::GetInstance().Init();

        Singleton<Application>::GetInstance().Run();
    }
} // namespace Vakol
