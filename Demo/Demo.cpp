// Vakol_Engine.cpp : Defines the entry point for the application.

#include "Layers/MyGUILayer.hpp"
#include "Vakol/core.hpp"

int main()
{
    MyGUILayer layer = MyGUILayer();
    Vakol::PushLayer(std::make_shared<MyGUILayer>(layer));
    Vakol::Run();

    return 0;
}
