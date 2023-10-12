#include "LayerManager/LayerManager.hpp"
#include "Logger/Logger.hpp"
#include "Utils/Layer.hpp"

#include "Window/Events/Event.hpp"

namespace Vakol
{

    void LayerManager::PushLayer(std::shared_ptr<Layer> layer, LayerSubscription flags)
    {
        m_layers.push_front(layer);

        if (flags & LayerFlags::UPDATES)
        {
            m_updateCallbacks.push_back(std::bind(&Layer::OnUpdate, layer));
            m_layerToUpdateMap[layer.get()] = --m_updateCallbacks.end();
            // store the iterator to the last element (whats being pushed)
        }

        if (flags & LayerFlags::EVENTS)
        {
            m_eventCallbacks.push_back(std::bind(&Layer::OnEvent, layer, std::placeholders::_1));
            m_layerToEventMap[layer.get()] = --m_eventCallbacks.end();
        }

        if (flags & LayerFlags::TICKS)
        {
            m_tickCallbacks.push_back(std::bind(&Layer::OnTick, layer));
            m_layerToTickMap[layer.get()] = --m_tickCallbacks.end();
        }
    }

    void LayerManager::PopLayer()
    {
        if (!m_layers.empty())
        {
            auto layer = m_layers.front();

            if (m_layerToUpdateMap.find(layer.get()) != m_layerToUpdateMap.end())
            {
                m_updateCallbacks.erase(m_layerToUpdateMap[layer.get()]);
                m_layerToUpdateMap.erase(layer.get());
            }

            if (m_layerToEventMap.find(layer.get()) != m_layerToEventMap.end())
            {
                m_eventCallbacks.erase(m_layerToEventMap[layer.get()]);
                m_layerToEventMap.erase(layer.get());
            }

            if (m_layerToTickMap.find(layer.get()) != m_layerToTickMap.end())
            {
                m_tickCallbacks.erase(m_layerToTickMap[layer.get()]);
                m_layerToTickMap.erase(layer.get());
            }

            layer->OnDetach();

            m_layers.pop_front();
        }
    }

    void LayerManager::OnUpdate()
    {
        for (auto& update : m_updateCallbacks)
        {
            update();
        }
    }

    void LayerManager::OnEvent(Event& event)
    {
        for (auto& eventCallback : m_eventCallbacks)
        {
            eventCallback(event);
            if (event.Handled)
                break;
        }
    }

    void LayerManager::OnTick()
    {
        for (auto& tick : m_tickCallbacks)
        {
            tick();
        }
    }

} // namespace Vakol