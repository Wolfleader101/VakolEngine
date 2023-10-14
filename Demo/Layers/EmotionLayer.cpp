#include "EmotionLayer.hpp"

#include <ECS/Components.hpp>
#include <Scripting/ScriptTypes.hpp>

#include <imgui.h>

#include <Logger/Logger.hpp>

void EmotionLayer::OnAttach()
{
}

void EmotionLayer::OnDetach()
{
}

void EmotionLayer::OnUpdate()
{
    if (m_app.GetGameState() == Vakol::GameState::Paused)
    {
        float m_values[7] = {0.5f, 0.9f, 0.2f, 0.8f, 0.1f, 0.7f, 0.6f};

        constexpr ImVec4 dark(0.15f, 0.15f, 0.15f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, dark);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.9f, 0.9f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_TitleBg, dark);
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, dark);

        ImGui::Begin("Entity Emotion Data");

        ImGui::PlotLines("Emotion", m_values, IM_ARRAYSIZE(m_values));

        ImGui::End();
        ImGui::PopStyleColor(4);
    }
}

void EmotionLayer::AddEmotionData(uint32_t EntityHandle, const std::array<float, 8>& data)
{
}

void EmotionLayer::OnTick()
{

    auto& EL = m_app.GetSceneManager().GetActiveScene().GetEntityList();

    EL.Iterate<Vakol::ScriptComp>([&](auto handle, auto& scripts) {
        for (const Vakol::LuaScript& script : scripts)
        {
            if (script.name = "emotions") // under assumption but does the job
            {
            }
        }
    });
}
