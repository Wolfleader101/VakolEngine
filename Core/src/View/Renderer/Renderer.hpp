#pragma once

#include <Controller/Time.hpp>
#include <View/Window/Window.hpp>
#include <Model/UniformBuffer.hpp>

#include <memory>

#include "Controller/Camera.hpp"

namespace Vakol::Model::Components 
{
    struct Drawable;
    struct Transform;
}

namespace Vakol::View 
{
    class Renderer 
    {
    public:
        Renderer(const std::shared_ptr<Window> window) : m_window(window){};
        virtual ~Renderer() {}

        virtual void AddUniform(const int size, const int binding) = 0;
        virtual void SetUniformData(const int index, const int offset, const int size, const void* data) const = 0;

        virtual void ClearColor(const glm::vec4& color) const = 0;
        virtual void ClearColor(const float r, const float g, const float b, const float a) const = 0;

        virtual void ClearBuffer(const unsigned int buffer_bit) const = 0;

        virtual void Draw(const Controller::Time& time, const Controller::Camera& camera,
                          const Model::Components::Transform, const Model::Components::Drawable& drawable) const = 0;

        virtual void Update() const = 0;

    protected:
        std::shared_ptr<Window> m_window = nullptr;
        std::vector<std::shared_ptr<UniformBuffer>> m_uniforms;
    };
}
