#include <Controller/Application.hpp>
#include <Controller/Singleton.hpp>
#include <iostream>

namespace Vakol {

void Run() {
    Controller::Singleton<Controller::Application>::GetInstance().Init();

    VK_TRACE("Hello from core");

    Controller::Singleton<Controller::Application>::GetInstance().Run();
}
}  // namespace Vakol
