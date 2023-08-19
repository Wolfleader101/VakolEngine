#pragma once

namespace Vakol
{
    class SceneManager;
    class Event;

    class Layer
    {
      public:
        Layer() : m_SceneManager(nullptr){};
        virtual ~Layer() = default;
        virtual void OnAttach(SceneManager* SM) = 0;
        virtual void OnDetach() = 0;
        virtual void OnUpdate() = 0;
        virtual void OnEvent(Event& event) = 0;
        virtual void OnGUI() = 0;
        virtual void OnTick() = 0;

      protected:
        SceneManager* m_SceneManager;
    };
} // namespace Vakol
