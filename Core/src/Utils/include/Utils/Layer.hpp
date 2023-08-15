#pragma once

namespace Vakol
{
    class SceneManager;
    class Event;

    class Layer
    {
      public:
        Layer(SceneManager& sceneManager) : m_SceneManager(sceneManager){};
        virtual void OnAttach() = 0;
        virtual void OnDetach() = 0;
        virtual void OnUpdate() = 0;
        virtual void OnEvent(Event& event) = 0;
        virtual void onGUI() = 0;
        virtual void onTick() = 0;

      protected:
        SceneManager& m_SceneManager;
    };
} // namespace Vakol
