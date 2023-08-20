#pragma once

#include <deque>
#include <memory>

namespace Vakol
{
    class Event;
    class Layer;

    class LayerManager
    {
      public:
        /**
         * @brief Construct a new Layer Manager object
         *
         */
        LayerManager() = default;

        /**
         * @brief Destroy the Layer Manager object
         *
         */
        ~LayerManager() = default;

        /**
         * @brief Push a layer to the layer stack
         *
         * @param layer
         */
        void PushLayer(std::shared_ptr<Layer> layer);

        /**
         * @brief Pop the top layer from the layer stack
         *
         */
        void PopLayer();

        /**
         * @brief Update the layers
         *
         */
        void OnUpdate();

        /**
         * @brief Handle events
         *
         * @param event
         */
        void OnEvent(Event& event);

        /**
         * @brief Handle GUI
         *
         */
        void OnGUI();

        /**
         * @brief Handle ticks
         *
         */
        void OnTick();

      private:
        /**
         * @brief the layers of the application.
         *
         *
         */
        std::deque<std::shared_ptr<Layer>> m_layers;
    };
} // namespace Vakol