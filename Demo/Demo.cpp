#include <memory>

#include "Layers/EmotionLayer.hpp"
#include "Layers/MyGUILayer.hpp"

#include "Vakol/core.hpp"

int main()
{

    Vakol::Init();

    Vakol::Application app = Vakol::Application();

    app.PushLayer(std::make_shared<MyGUILayer>(app), Vakol::LayerFlags::UPDATES | Vakol::LayerFlags::EVENTS);
    app.PushLayer(std::make_shared<EmotionLayer>(app), Vakol::LayerFlags::UPDATES | Vakol::LayerFlags::TICKS);

    app.Run();

    return 0;
}
