#pragma once

#include <Model/Buffer.hpp>
#include <Model/Skybox.hpp>

#include "Renderer.hpp"

namespace Vakol::View
{
    using Model::Buffer;
    using Model::FrameBuffer;
    using Model::Skybox;

    /**
     * @brief GLRenderer class derived from Renderer, for rendering in OpenGL
     */
    class GLRenderer final : public Renderer
    {
      public:
        /**
         * @brief GLRenderer constructor
         * @param window shared pointer to a Window object
         */
        explicit GLRenderer(const std::shared_ptr<Window>& window);

        /**
         * @brief Add a buffer to the GLRenderer
         * @param type the type of buffer, such as GL_UNIFORM_BUFFER or GL_SHADER_STORAGE_BUFFER
         * @param size the size of the buffer in bytes
         * @param binding the index at which the buffer is bound
         * @param usage OpenGL usage flag, often GL_STATIC_DRAW
         */
        void AddBuffer(unsigned int type, int size, int binding, unsigned int usage) override;

        /**
         * @brief Add a buffer to the GLRenderer with initial data
         * @param type the type of buffer, such as GL_UNIFORM_BUFFER or GL_SHADER_STORAGE_BUFFER
         * @param size the size of the buffer in bytes
         * @param binding the index at which the buffer is bound
         * @param data pointer to initial data for the buffer
         * @param usage OpenGL usage flag, often GL_STATIC_DRAW
         */
        void AddBuffer(unsigned int type, int size, int binding, const void* data, unsigned int usage) override;

        /**
         * @brief Bind a FrameBuffer
         * @param index optional index of the FrameBuffer to bind, default is 0
         */
        void BindFrameBuffer(const int index = 0) const
        {
            if (static_cast<int>(framebuffers.size()) > index)
                framebuffers.at(index)->Bind();
        }

        /**
         * @brief Unbind a FrameBuffer
         * @param index optional index of the FrameBuffer to unbind, default is 0
         */
        void UnbindFrameBuffer(const int index = 0) const
        {
            if (static_cast<int>(framebuffers.size()) > index)
                framebuffers.at(index)->Unbind();
        }

        /**
         * @brief Set subdata in a buffer
         * @param index index of the buffer in GLRenderer's vector of buffers
         * @param offset byte offset of the data
         * @param size size of the data in bytes
         * @param data pointer to the data
         */
        void SetBufferSubData(int index, int offset, int size, const void* data) const override;

        /**
         * @brief Clear color buffer with a color
         * @param color optional glm::vec4 specifying the color, default is black with full opacity
         */
        void ClearColor(const glm::vec4& color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) const override;

        /**
         * @brief Clear color buffer with a color specified by four floats
         * @param r red component
         * @param g green component
         * @param b blue component
         * @param a optional alpha component, default is full opacity
         */
        void ClearColor(float r, float g, float b, float a = 1.0f) const override;

        /**
         * @brief Clear a buffer
         * @param buffer_bit bit flag specifying the buffer to clear
         */
        void ClearBuffer(unsigned int buffer_bit) const override;

        /**
         * @brief Draw a model with animation
         * @param transform transform component of the model
         * @param drawable drawable component of the model
         * @param animation animation asset of the model
         */
        void DrawAnimated(const Model::Components::Transform& transform, const Model::Components::Drawable& drawable,
                          const Model::Assets::Animation& animation) const override;

        /**
         * @brief Draw a model without animation
         * @param transform transform component of the model
         * @param drawable drawable component of the model
         */
        void Draw(const Model::Components::Transform& transform,
                  const Model::Components::Drawable& drawable) const override;

        /**
         * @brief Update data in the renderer based on a camera
         * @param camera the camera to update data from
         */
        void UpdateData(const Controller::Camera& camera) override;

        /**
         * @brief Perform update at index
         * @param index the index to perform update at
         */
        void Update(int index) const override;

        /**
         * @brief Perform late update at index
         * @param index the index to perform late update at
         */
        void LateUpdate(int index) const override;

        /**
         * @brief Toggle wireframe mode on and off
         */
        void ToggleWireframe() override
        {
            isWireframe = !isWireframe;

            if (isWireframe)
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            else
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        void ToggleSkybox() override
        {
            isSkybox = !isSkybox;
        }

        void SetWireframe(const bool wireframe) override
        {
            isWireframe = wireframe;

            if (isWireframe)
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            else
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        void SetSkybox(const bool showSkybox) override
        {
            isSkybox = showSkybox;
        }

      private:
        std::vector<std::shared_ptr<Buffer>> buffers;           ///< Vector of shared pointers to Buffer objects
        std::vector<std::shared_ptr<FrameBuffer>> framebuffers; ///< Vector of shared pointers to FrameBuffer objects
        std::shared_ptr<Skybox> skybox = std::make_shared<Skybox>(); ///< Shared pointer to a Skybox object
    };
} // namespace Vakol::View
