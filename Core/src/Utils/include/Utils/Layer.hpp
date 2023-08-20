#pragma once

namespace Vakol
{
    class SceneManager;
    class Event;

    /**
     * @brief Interface for layers
     *
     */
    class Layer
    {
      public:
        /**
         * @brief Construct a new Layer object
         *
         */
        Layer() : m_SceneManager(nullptr){};

        /**
         * @brief Destroy the Layer object
         *
         */
        virtual ~Layer() = default;

        /**
         * @brief Called when the layer is attached to the scene manager
         *
         * @param SM
         */
        virtual void OnAttach(SceneManager* SM) = 0;

        /**
         * @brief Called when the layer is detached from the scene manager
         *
         */
        virtual void OnDetach() = 0;

        /**
         * @brief Called every frame
         *
         */
        virtual void OnUpdate() = 0;

        /**
         * @brief Called when an event is triggered
         *
         * @param event
         */
        virtual void OnEvent(Event& event) = 0;

        /**
         * @brief Called every tick
         *
         */
        virtual void OnTick() = 0;

      protected:
        /**
         * @brief Pointer to the scene manager
         *
         */
        SceneManager* m_SceneManager;
    };
} // namespace Vakol
