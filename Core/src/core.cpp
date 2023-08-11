#include "../include/Vakol/core.hpp"

#include "Application/Application.hpp"
#include "Utils/Singleton.hpp"
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
