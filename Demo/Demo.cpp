#include <memory>

#include "Layers/MyGUILayer.hpp"
#include "Vakol/core.hpp"

int main()
{

    Vakol::Init();

    Vakol::Application app = Vakol::Application();

    app.PushLayer(std::make_shared<MyGUILayer>(app), Vakol::LayerFlags::UPDATES | Vakol::LayerFlags::EVENTS);

    app.Run();

    return 0;
}
