#include "../include/Vakol/core.hpp"

#include "Application/Application.hpp"
#include "Utils/Singleton.hpp"
#include <iostream>

namespace Vakol
{
    void Init()
    {
        Logger::Init();
        Singleton<Application>::GetInstance().Init();
    }

    void PushLayer(std::shared_ptr<Layer> layer)
    {
        Singleton<Application>::GetInstance().PushLayer(layer);
    }

    void Run()
    {
        Singleton<Application>::GetInstance().Run();
    }

} // namespace Vakol
