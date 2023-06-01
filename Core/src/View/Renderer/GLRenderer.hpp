#pragma once

#include "Renderer.hpp"

#include <Model/Buffer.hpp>
#include <Model/Skybox.hpp>

namespace Vakol::View 
{
    using Model::Skybox;

    using Model::Buffer;
    using Model::FrameBuffer;

    class GLRenderer final : public Renderer 
    {
    public:
	    explicit GLRenderer(const std::shared_ptr<Window>& window);

        /// @brief x
        /// @param type the type of buffer, GL_UNIFORM_BUFFER and GL_SHADER_STORAGE_BUFFER are the ones you're looking for
        /// @param size the size of the buffer (in bytes)
        /// @param binding the index at which 
        /// @param usage how OpenGL should handle the buffer data (mainly using GL_STATIC_DRAW (set once, modify many times))
        void AddBuffer(unsigned int type, int size, int binding, unsigned int usage) override;

        // Same function as previous, allows the user to pre-store data in the buffer beforehand (useful if you don't need to update the data every frame)
        void AddBuffer(unsigned int type, int size, int binding, const void* data, unsigned int usage) override;

        void BindFrameBuffer(const int index = 0) const { if (static_cast<int>(framebuffers.size()) > index) framebuffers.at(index)->Bind(); }
        void UnbindFrameBuffer(const int index = 0) const { if (static_cast<int>(framebuffers.size()) > index) framebuffers.at(index)->Unbind(); }

        /// @brief x
        /// @param index the index in the GLRenderer buffer vector at which the element resides in
        /// @param offset the byte offset at which the element occurs. for example: if we stored two floats and I wanted to access the first float value, the offset would be 0. If I wanted the second float value then the offset would be sizeof(float).
        /// @param size size of the element
        /// @param data stuff
        void SetBufferSubData(int index, int offset, int size, const void* data) const override;

    	void ClearColor(const glm::vec4& color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) const override;
    	void ClearColor(float r, float g, float b, float a = 1.0f) const override;

    	void ClearBuffer(unsigned int buffer_bit) const override;

        void Draw(const Controller::Time& time, const Controller::Camera& camera, const Model::Components::Transform& transform, const Model::Components::Drawable& drawable) const override;
        void UpdateData(const Controller::Time& time, const Controller::Camera& camera) override;

        void Update(int index) const override;
        void LateUpdate(int index) const override;

        void ToggleWireframe() override 
        {
            isWireframe = !isWireframe;

            if (isWireframe)
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            else
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        void ToggleSkybox() override  { isSkybox = !isSkybox; }
    private:
        std::vector<std::shared_ptr<Buffer>> buffers;
        std::vector<std::shared_ptr<FrameBuffer>> framebuffers;

        std::shared_ptr<Skybox> skybox = std::make_shared<Skybox>();
    };
}
