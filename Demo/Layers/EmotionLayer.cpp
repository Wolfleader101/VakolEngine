#include "EmotionLayer.hpp"

#include <ECS/Components.hpp>
#include <Scripting/ScriptTypes.hpp>

#include <imgui.h>

#include "implot.h"

#include <Logger/Logger.hpp>

void EmotionLayer::OnAttach()
{
    ImPlot::CreateContext();
}

void EmotionLayer::OnDetach()
{
    ImPlot::DestroyContext();
}

void EmotionLayer::OnUpdate()
{
    if (m_show)
    {
        auto& EL = m_app.GetSceneManager().GetActiveScene().GetEntityList();

        constexpr ImVec4 dark(0.15f, 0.15f, 0.15f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, dark);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.9f, 0.9f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_TitleBg, dark);
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, dark);

        ImGui::Begin("Entity Emotion Data");

        for (const auto& iter : m_EmotionData)
        {
            const std::string& entityName = EL.GetEntity(iter.first).GetComponent<Vakol::Components::Tag>().tag;

            if (ImGui::CollapsingHeader(entityName.c_str()))
            {

                if (ImPlot::BeginPlot("##EmotionPlot", nullptr, nullptr, ImVec2(-1, 0), 0, ImPlotAxisFlags_NoTickLabels,
                                      ImPlotAxisFlags_NoTickLabels))
                {
                    ImPlot::SetupAxesLimits(0, m_BuffSize, -1.0, 1.0);
                    for (int i = 0; i < 8; i++)
                    {
                        std::vector<float> vec(iter.second[i].begin(), iter.second[i].end());
                        ImPlot::PlotLine(m_EmotionNames[i].c_str(), vec.data(), vec.size());
                    }

                    ImPlot::EndPlot();
                }
            }
        }

        ImGui::End();
        ImGui::PopStyleColor(4);
    }
}

void EmotionLayer::OnTick()
{

    if (tickCount != tickSkips)
    {
        tickCount++;
        return;
    }
    auto& EL = m_app.GetSceneManager().GetActiveScene().GetEntityList();

    EL.Iterate<Vakol::ScriptComp>([&](auto handle, Vakol::ScriptComp& scripts) {
        for (const Vakol::LuaScript& script : scripts.scripts)
        {
            if (script.name == "emotions") // under assumption but does the job
            {
                auto func = script.env["get_emotion"];

                for (int i = 0; i < 8; i++)
                {
                    auto& emotion_arr = m_EmotionData[static_cast<uint32_t>(handle)];
                    emotion_arr[i].push_back(func(i + 1));

                    if (emotion_arr[i].size() > m_BuffSize)
                    {
                        emotion_arr[i].pop_front();
                    }
                }
            }
        }
    });

    tickCount = 0;
}

void EmotionLayer::OnEvent(Vakol::Event& event)
{
    if (event.GetEventType() == Vakol::EventType::KeyPressed)
    {
        auto& keyEvent = dynamic_cast<Vakol::KeyPressedEvent&>(event);

        if (keyEvent.GetKeyCode() == static_cast<int>(Vakol::Input::KEY::KEY_0))
        {
            m_show = !m_show;
            event.Handled = true;
            return;
        }

        if (keyEvent.GetKeyCode() == static_cast<int>(Vakol::Input::KEY::KEY_ESCAPE))
        {
            m_show = false;
            m_app.SetGameState(Vakol::GameState::Running);
        }
    }
}
