#include "MyGUILayer.hpp"
#include <iostream>

void MyGUILayer::OnAttach(Vakol::SceneManager* SM)
{
    m_SceneManager = SM;
}

void MyGUILayer::OnDetach()
{
}

void MyGUILayer::OnUpdate()
{
    std::cout << "MyGUILayer::OnUpdate()" << std::endl;
}

void MyGUILayer::OnEvent(Vakol::Event& event)
{
}

void MyGUILayer::OnGUI()
{
}

void MyGUILayer::OnTick()
{
}
