#pragma once

#include <deque>
#include <memory>

namespace Vakol
{
    class Event;
    class Layer;

    /**
     * @brief orders layers as a stack and sends information through as such.
     *
     */
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
         * @param layer to push
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
         * @param event to handle
         */
        void OnEvent(Event& event);

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