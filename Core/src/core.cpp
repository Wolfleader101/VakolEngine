#include "../include/Vakol/core.hpp"

#include "Application/include/Application.hpp"
#include "Controller/Singleton.hpp"
#include <iostream>

namespace Vakol
{
    void Run()
    {
        Controller::Logger::Init();

        Controller::Singleton<Application>::GetInstance().Init();

        Controller::Singleton<Application>::GetInstance().Run();
    }
} // namespace Vakol
