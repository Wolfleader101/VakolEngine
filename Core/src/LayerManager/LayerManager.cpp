#include "LayerManager/LayerManager.hpp"
#include "Logger/Logger.hpp"
#include "Utils/Layer.hpp"

namespace Vakol
{

    void LayerManager::PushLayer(std::shared_ptr<Layer> layer)
    {
        m_layers.push_back(layer);
    }

    void LayerManager::PopLayer()
    {
        if (!m_layers.empty())
            m_layers.pop_back();
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
        for (auto& layer : m_layers)
        {
            layer->OnEvent(event);
        }
    }

    void LayerManager::OnGUI()
    {
        for (auto& layer : m_layers)
        {
            layer->OnGUI();
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