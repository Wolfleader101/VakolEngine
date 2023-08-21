#include "LayerManager/LayerManager.hpp"
#include "Logger/Logger.hpp"
#include "Utils/Layer.hpp"

#include "Window/Events/Event.hpp"

namespace Vakol
{

    void LayerManager::PushLayer(std::shared_ptr<Layer> layer)
    {
        m_layers.push_front(layer);
    }

    void LayerManager::PopLayer()
    {
        if (!m_layers.empty())
        {
            m_layers.front()->OnDetach();
            m_layers.pop_front();
        }
    }

    void LayerManager::OnUpdate()
    {
        for (auto& layer : m_layers)
        {
            layer->OnUpdate();
        }
    }

    void LayerManager::OnEvent(Event& event)
    {
        auto it = m_layers.begin();

        while (it != m_layers.end() && !event.Handled)
        {
            (*it)->OnEvent(event);
            it++;
        }
    }

    void LayerManager::OnTick()
    {
        for (auto& layer : m_layers)
        {
            layer->OnTick();
        }
    }

} // namespace Vakol