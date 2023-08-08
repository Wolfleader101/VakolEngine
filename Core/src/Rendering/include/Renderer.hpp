#pragma once

#include <memory>

#include "Controller/Time.hpp"
#include "Window/include/Window.hpp"

#include "Math/include/Math.hpp"

#include "Controller/Camera.hpp"

namespace Vakol::Components
{
    struct Drawable;
    struct Transform;
} // namespace Vakol::Components

namespace Vakol::Model::Assets
{
    class Animation;
}

namespace Vakol
{
    /**
     * @brief Renderer class for abstracting rendering operations
     */
    class Renderer
    {
      public:
        /**
         * @brief Renderer constructor
         * @param window shared pointer to a Window object
         */
        explicit Renderer(const std::shared_ptr<Window>& window) : m_window(window)
        {
        }

        /**
         * @brief Virtual Renderer destructor
         */
        virtual ~Renderer() = default;

        /**
         * @brief Draw a model with animation
         * @param transform transform component of the model
         * @param drawable drawable component of the model
         * @param animation animation asset of the model
         */
        virtual void DrawAnimated(const Components::Transform& transform, const Components::Drawable& drawable,
                                  const Model::Assets::Animation& animation) const = 0;

        /**
         * @brief Draw a model without animation
         * @param transform transform component of the model
         * @param drawable drawable component of the model
         */
        virtual void Draw(const Components::Transform& transform, const Components::Drawable& drawable) const = 0;

        /**
         * @brief Update data in the renderer based on a camera
         * @param camera the camera to update data from
         */
        virtual void UpdateData(const Controller::Camera& camera) = 0;

        /**
         * @brief Perform update at index
         * @param index optional index to perform update at, default is -1
         */
        virtual void Update(int index = -1) const = 0;

        /**
         * @brief Perform late update at index
         * @param index optional index to perform late update at, default is -1
         */
        virtual void LateUpdate(int index = -1) const = 0;

        /**
         * @brief Add a buffer to the Renderer
         * @param type the type of buffer, such as GL_UNIFORM_BUFFER or GL_SHADER_STORAGE_BUFFER
         * @param size the size of the buffer in bytes
         * @param binding the index at which the buffer is bound
         * @param usage OpenGL usage flag, often GL_STATIC_DRAW
         */
        virtual void AddBuffer(unsigned int type, int size, int binding, unsigned int usage) = 0;

        /**
         * @brief Add a buffer to the Renderer with initial data
         * @param type the type of buffer, such as GL_UNIFORM_BUFFER or GL_SHADER_STORAGE_BUFFER
         * @param size the size of the buffer in bytes
         * @param binding the index at which the buffer is bound
         * @param data pointer to initial data for the buffer
         * @param usage OpenGL usage flag, often GL_STATIC_DRAW
         */
        virtual void AddBuffer(unsigned int type, int size, int binding, const void* data, unsigned int usage) = 0;

        /**
         * @brief Set subdata in a buffer
         * @param index index of the buffer in Renderer's vector of buffers
         * @param offset byte offset of the data
         * @param size size of the data in bytes
         * @param data pointer to the data
         */
        virtual void SetBufferSubData(int index, int offset, int size, const void* data) const = 0;

        /**
         * @brief Clear color buffer with a specified color
         * @param color the color to clear the color buffer with, default is black with full alpha
         */
        virtual void ClearColor(const Math::Vec4& color = Math::Vec4(0.0f, 0.0f, 0.0f, 1.0f)) const = 0;

        /**
         * @brief Clear color buffer with a specified color
         * @param r red component of the color
         * @param g green component of the color
         * @param b blue component of the color
         * @param a alpha component of the color, default is 1.0f
         */
        virtual void ClearColor(float r, float g, float b, float a = 1.0f) const = 0;

        /**
         * @brief Clear specified buffer
         * @param buffer_bit the bit mask identifying the buffer to be cleared
         */
        virtual void ClearBuffer(unsigned int buffer_bit) const = 0;

        /**
         * @brief Toggle wireframe mode on and off
         */
        virtual void ToggleWireframe() = 0;

        /**
         * @brief Toggle skybox on and off
         */
        virtual void ToggleSkybox() = 0;

        virtual void SetWireframe(const bool wireframe) = 0;
        virtual void SetSkybox(const bool skybox) = 0;

      protected:
        bool isWireframe = false; ///< Boolean flag indicating whether wireframe mode is on
        bool isSkybox = true;     ///< Boolean flag indicating whether skybox is on

        std::shared_ptr<Window> m_window = nullptr; ///< Shared pointer to a Window object
    };
} // namespace Vakol
