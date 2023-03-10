#include "../include/Vakol/core.hpp"

#include <Controller/Application.hpp>
#include <Controller/Singleton.hpp>
#include <iostream>

namespace Vakol {

    void Run() {
        Controller::Singleton<Controller::Application>::GetInstance().Init();

        Controller::Singleton<Controller::Application>::GetInstance().Run();
    }
}  // namespace Vakol
