#pragma once

namespace Vakol
{
    class Event;
    class Application;

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
        Layer(Application& app) : m_app(app)
        {
        }

        /**
         * @brief Destroy the Layer object
         *
         */
        virtual ~Layer() = default;

        /**
         * @brief Called when the layer is attached to the scene manager
         */
        virtual void OnAttach() = 0;

        /**
         * @brief Called when the layer is detached from the scene manager
         *
         */
        virtual void OnDetach() = 0;

        /**
         * @brief Called every frame
         *
         */
        virtual inline void OnUpdate()
        {
        }

        /**
         * @brief Called when an event is triggered
         *
         * @param event to handle
         */
        virtual inline void OnEvent(Event& event)
        {
        }

        /**
         * @brief Called every tick
         *
         */
        virtual inline void OnTick()
        {
        }

      protected:
        /**
         * @brief injected application reference
         *
         */
        Application& m_app;
    };

    /**
     * @brief enum class for layer subscriptions
     *
     */
    enum LayerFlags : unsigned int
    {
        UPDATES = 1 << 0,
        TICKS = 1 << 1,
        EVENTS = 1 << 2
    };
} // namespace Vakol
