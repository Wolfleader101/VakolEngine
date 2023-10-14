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

  private:
    void AddEmotionData(uint32_t EntityHandle, const std::array<float, 8>& data);

    unsigned m_BuffSize = 30;
    std::unordered_map<uint32_t, std::deque<std::array<float, 8>>> m_EmotionData;
    // hows that for a data structure adlay

    const std::array<std::string, 8> m_EmotionNames = {"Anger", "Anticipation", "Joy",     "Trust",
                                                       "Fear",  "Surprise",     "Sadness", "Disgust"};
};