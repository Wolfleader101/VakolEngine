#pragma once

#include <deque>
#include <functional>
#include <memory>
#include <unordered_map>

namespace Vakol
{
    class Event;
    class Layer;

    using LayerSubscription = unsigned int;

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
         * @param flags to subscribe to
         */
        void PushLayer(std::shared_ptr<Layer> layer, LayerSubscription flags);

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

        //-------------- Callbacks -----------------//

        /**
         * @brief the layers that are subscribed to updates.
         *
         */
        std::vector<std::function<void()>> m_updateCallbacks;

        /**
         * @brief the layers that are subscribed to events.
         *
         */
        std::vector<std::function<void(Event&)>> m_eventCallbacks;

        /**
         * @brief the layers that are subscribed to ticks.
         *
         */
        std::vector<std::function<void()>> m_tickCallbacks;

        //-------------- Maps -----------------//

        /**
         * @brief maps layers to their update callback.
         *
         */
        std::unordered_map<Layer*, std::vector<std::function<void()>>::iterator> m_layerToUpdateMap;

        /**
         * @brief maps layers to their event callback.
         *
         */
        std::unordered_map<Layer*, std::vector<std::function<void(Event&)>>::iterator> m_layerToEventMap;

        /**
         * @brief maps layers to their tick callback.
         *
         */
        std::unordered_map<Layer*, std::vector<std::function<void()>>::iterator> m_layerToTickMap;
    };
} // namespace Vakol