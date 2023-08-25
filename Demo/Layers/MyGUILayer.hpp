#pragma once

#include "Utils/Layer.hpp"

#include <imgui.h>

class MyGUILayer : public Vakol::Layer
{
  public:
    MyGUILayer() = default;
    ~MyGUILayer() = default;

    void OnAttach(Vakol::SceneManager* SM) override;
    void OnDetach() override;
    void OnUpdate() override;
    void OnEvent(Vakol::Event& event) override;
    void OnTick() override;

  private:
    bool m_Show = false;
};
