#pragma once

#include <Controller/Time.hpp>
#include <View/Window/Window.hpp>
#include <memory>

#include "Controller/Camera.hpp"

namespace Vakol::Model::Components {
    struct Drawable;
    struct Transform;
}  // namespace Vakol::Model::Components
namespace Vakol::View {
    class Renderer {
       public:
        Renderer(const std::shared_ptr<Window> window) : m_window(window){};
        virtual ~Renderer() {}

        virtual void Draw(const Controller::Time& time, const Controller::Camera& camera,
                          const Model::Components::Transform, const Model::Components::Drawable& drawable) const = 0;

        virtual void Update() const = 0;

       protected:
        std::shared_ptr<Window> m_window;
    };
}  // namespace Vakol::View
