#pragma once

#include <Controller/Time.hpp>
#include <View/Window/Window.hpp>
#include <memory>

#include "Controller/Camera.hpp"

namespace Vakol::Model::Components 
{
    struct Drawable;
    struct Transform;
}  // namespace Vakol::Model::Components

namespace Vakol::View 
{
    class Renderer 
    {
       public:
	    explicit Renderer(const std::shared_ptr<Window>& window) : m_window(window) {}
        virtual ~Renderer() {}

        virtual void Draw(const Controller::Time& time, const Controller::Camera& camera, const Model::Components::Transform& transform, const Model::Components::Drawable& drawable) const = 0;

        virtual void Update() const = 0;

        virtual void ToggleWireframe() = 0;

    protected:
        bool isWireframe = false;
        std::shared_ptr<Window> m_window = nullptr;
    };
}
