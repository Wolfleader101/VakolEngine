#pragma once

#include "Renderer.hpp"

#include <Model/Buffer.hpp>
#include <Model/Skybox.hpp>

namespace Vakol::View 
{
    using Model::Skybox;
    using Model::Buffer;

    class GLRenderer : public Renderer 
    {
    public:
        GLRenderer(const std::shared_ptr<Window>& window);

        /// @brief 
        /// @param type the type of buffer, GL_UNIFORM_BUFFER and GL_SHADER_STORAGE_BUFFER are the ones you're looking for
        /// @param size the size of the buffer (in bytes)
        /// @param binding the index at which 
        /// @param usage how OpenGL should handle the buffer data (mainly using GL_STATIC_DRAW (set once, modify many times))
        void AddBuffer(const unsigned int type, const int size, const int binding, const unsigned int usage);

        // Same function as previous, allows the user to pre-store data in the buffer beforehand (useful if you don't need to update the data every frame)
        void AddBuffer(const unsigned int type, const int size, const int binding, const void* data, const unsigned int usage);

        /// @brief 
        /// @param index the index in the GLRenderer buffer vector at which the element resides in
        /// @param offset the byte offset at which the element occurs.
        /// for example: if we stored two floats and I wanted to access the first float value, the offset would be 0.
        /// If I wanted the second float value then the offset would be sizeof(float).
        /// @param size size of the element
        /// @param data 
        void SetBufferSubData(const int index, const int offset, const int size, const void* data) const;

        static void ClearColor(const glm::vec4& color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        static void ClearColor(const float r, const float g, const float b, const float a = 1.0f);

        static void ClearBuffer(const unsigned int buffer_bit);

        void Draw([[maybe_unused]] const Controller::Time& time, const Controller::Camera& camera, const Model::Components::Transform, const Model::Components::Drawable& drawable) const override;

        void Update() const override;

        inline void ToggleWireframe() override 
        {
            isWireframe = !isWireframe;

            if (isWireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
        }
    private:
        std::vector<std::shared_ptr<Buffer>> buffers;
        std::shared_ptr<Skybox> skybox = std::make_shared<Skybox>();
    };
}
