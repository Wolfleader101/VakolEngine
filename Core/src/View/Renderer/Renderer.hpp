#pragma once

#include <memory>

#include <Controller/Time.hpp>
#include <View/Window/Window.hpp>

namespace Vakol::View 
{
    class Renderer 
    {
    public:
        Renderer(const std::shared_ptr<Window> window) : m_window(window){};
        virtual ~Renderer() {}

        virtual void Update(const Controller::Time& time) = 0;

    protected:
        std::shared_ptr<Window> m_window;
    };
}