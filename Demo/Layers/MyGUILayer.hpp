#pragma once

#include "Utils/Layer.hpp"

class MyGUILayer : public Vakol::Layer
{
  public:
    MyGUILayer() = default;
    ~MyGUILayer() = default;

    void OnAttach(Vakol::SceneManager* SM) override;
    void OnDetach() override;
    void OnUpdate() override;
    void OnEvent(Vakol::Event& event) override;
    void OnGUI() override;
    void OnTick() override;
};
