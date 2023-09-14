#include <memory>

#include "Layers/MyGUILayer.hpp"
#include "Vakol/core.hpp"

int main()
{

    Vakol::Init();

    Vakol::Application app = Vakol::Application();

    app.Init();

    app.PushLayer(std::make_shared<MyGUILayer>(app));

    app.Run();

    return 0;
}
