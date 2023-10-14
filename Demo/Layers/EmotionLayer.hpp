#pragma once

#include <Application/Application.hpp>
#include <ECS/Entity.hpp>
#include <Utils/Layer.hpp>
#include <array>
#include <deque>
#include <string>
#include <unordered_map>
class EmotionLayer : public Vakol::Layer
{
  public:
    EmotionLayer(Vakol::Application& app) : Vakol::Layer(app)
    {
    }

    ~EmotionLayer() = default;

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate() override;
    void OnTick() override;
    void OnEvent(Vakol::Event& event) override;

  private:
    void AddEmotionData(uint32_t EntityHandle, const std::array<float, 8>& data);

    bool m_show;

    const int tickSkips = 5;
    int tickCount = 0;

    const unsigned m_BuffSize = 15;
    std::unordered_map<uint32_t, std::array<std::deque<float>, 8>> m_EmotionData;

    const std::array<std::string, 8> m_EmotionNames = {"Anger", "Anticipation", "Joy",     "Trust",
                                                       "Fear",  "Surprise",     "Sadness", "Disgust"};
};