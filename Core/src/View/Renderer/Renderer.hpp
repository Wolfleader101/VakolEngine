#pragma once

#include <Controller/Time.hpp>
#include <View/Window/Window.hpp>
#include <memory>

#include "Model/Components.hpp"

namespace Vakol::View {
    class Renderer {
       public:
        Renderer(const std::shared_ptr<Window> window) : m_window(window){};
        virtual ~Renderer() {}

        virtual void Draw(const Controller::Time& time,
                          const std::shared_ptr<Model::Components::Drawable> drawable) = 0;
        virtual void Update(const Controller::Time& time) = 0;

       protected:
        std::shared_ptr<Window> m_window;
    };
}  // namespace Vakol::View
