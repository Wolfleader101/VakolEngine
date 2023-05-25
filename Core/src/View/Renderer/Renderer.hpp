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
        virtual ~Renderer() = default;

        virtual void Draw(const Controller::Time& time, const Controller::Camera& camera, const Model::Components::Transform& transform, const Model::Components::Drawable& drawable) const = 0;

        virtual void Update(int index = -1) const = 0;

        /// @brief x
        /// @param type the type of buffer, GL_UNIFORM_BUFFER and GL_SHADER_STORAGE_BUFFER are the ones you're looking for
        /// @param size the size of the buffer (in bytes)
        /// @param binding the index at which 
        /// @param usage how OpenGL should handle the buffer data (mainly using GL_STATIC_DRAW (set once, modify many times))
        virtual void AddBuffer(unsigned int type, int size, int binding, unsigned int usage) = 0;

        // Same function as previous, allows the user to pre-store data in the buffer beforehand (useful if you don't need to update the data every frame)
        virtual void AddBuffer(unsigned int type, int size, int binding, const void* data, unsigned int usage) = 0;

        /// @brief x
        /// @param index the index in the GLRenderer buffer vector at which the element resides in
        /// @param offset the byte offset at which the element occurs. for example: if we stored two floats and I wanted to access the first float value, the offset would be 0. If I wanted the second float value then the offset would be sizeof(float).
        /// @param size size of the element
        /// @param data stuff
        virtual void SetBufferSubData(int index, int offset, int size, const void* data) const = 0;

    	virtual void ClearColor(const glm::vec4& color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) const = 0;
        virtual void ClearColor(float r, float g, float b, float a = 1.0f) const = 0;

        virtual void ClearBuffer(unsigned int buffer_bit) const = 0;

        virtual void ToggleWireframe() = 0;

    protected:
        bool isWireframe = false;
        std::shared_ptr<Window> m_window = nullptr;
    };
}
