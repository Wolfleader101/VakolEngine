#pragma once

#include <memory>

#include "Animation.hpp"
#include "Mesh.hpp"

#include "Model/Shader.hpp"
#include "Model/Buffer.hpp"

namespace Vakol::Model::Assets
{
    class Model
	{
    public:
        Model() = default;

        Model(Mesh& mesh) { m_meshes.push_back(std::move(mesh)); }
        Model(Mesh&& mesh) { m_meshes.push_back(std::move(mesh)); }

        Model(std::vector<Mesh>&& meshes) : m_meshes(std::move(meshes)) {}
        Model(std::vector<Mesh>& meshes) : m_meshes(std::move(meshes)) {}

        Model(std::vector<Mesh>&& meshes, std::vector<Animation>&& animations)
            : m_meshes(std::move(meshes)), m_animations(std::move(animations)), m_animated(true) {}

        Model(std::vector<Mesh>& meshes, std::vector<Animation>& animations)
            : m_meshes(std::move(meshes)), m_animations(std::move(animations)), m_animated(true) {}

        void set_shader(const std::string& path)
    	{
            this->m_shader = std::make_shared<Shader>(path);
            this->m_shader->Bind();
        }

        [[nodiscard]] bool isAnimated() const { return m_animated; }

        std::shared_ptr<Shader>& shader() { return m_shader; }
        [[nodiscard]] const std::shared_ptr<Shader>& c_shader() const { return m_shader; }

        void UpdateAnimation(const float delta_time) { m_animations.at(m_animation_state).Update(delta_time); }

    	void ResetAnimation() { m_animations.at(m_animation_state).ResetAnimation(); }

    	void ResetAnimation(const int state)
    	{
        	VK_ASSERT(state < numAnimations() && state >= 0, "\n\nAnimation Index out of bounds!");

        	m_animations.at(state).ResetAnimation();
    	}

        void SetAnimationState(int state)
    	{
	        if (const auto size = numAnimations(); state < size && state >= 0)
                m_animation_state = state;
            else 
            {
                state = std::max(0, size - 1);
                m_animation_state = state;
            }
        }

        /// @brief x
        /// @param type the type of buffer, GL_UNIFORM_BUFFER and GL_SHADER_STORAGE_BUFFER are the ones you're looking for
        /// @param size the size of the buffer (in bytes)
        /// @param binding the index at which 
        /// @param usage how OpenGL should handle the buffer data (mainly using GL_STATIC_DRAW (set once, modify many times))
        void AddBuffer(const unsigned int type, const int size, const int binding, const unsigned int usage)
        {
        	m_buffers.push_back(std::make_shared<Buffer>(type, size, binding, usage));
        }

        // Same function as previous, allows the user to pre-store data in the buffer beforehand (useful if you don't need to update the data every frame)
        void AddBuffer(const unsigned int type, const int size, const int binding, const void* data, const unsigned int usage)
        {
	        m_buffers.push_back(std::make_shared<Buffer>(type, size, binding, data, usage));
        }

        /// @brief x
        /// @param index the index in the GLRenderer buffer vector at which the element resides in
        /// @param offset the byte offset at which the element occurs. for example: if we stored two floats and I wanted to access the first float value, the offset would be 0. If I wanted the second float value then the offset would be sizeof(float).
        /// @param size size of the element
        /// @param data stuff
        void SetBufferSubData(const int index, const int offset, const int size, const void* data) const
        {
	        m_buffers.at(index)->SetSubData(offset, size, data);
        }

        [[nodiscard]] int numAnimations() const { return static_cast<int>(m_animations.size()); }
        [[nodiscard]] int numTransforms() const { return m_animations.at(m_animation_state).numTransforms(); }

        [[nodiscard]] float animation_duration_s() const { return m_animations.at(m_animation_state).duration_s(); }
        [[nodiscard]] float animation_duration_ms() const { return m_animations.at(m_animation_state).duration_ms(); }

    	[[nodiscard]] const std::vector<glm::mat4>& transforms() const { return m_animations.at(m_animation_state).transforms(); }

        [[nodiscard]] const Mesh& mesh(const int index = 0) const { return m_meshes.at(index); }

        [[nodiscard]] const std::vector<Mesh>& c_meshes() const { return m_meshes; }
        [[nodiscard]] std::vector<Mesh>& meshes() { return m_meshes; }

        [[nodiscard]] int nMeshes() const { return static_cast<int>(m_meshes.size()); }

    private:
        std::vector<Mesh> m_meshes;
        std::vector<Animation> m_animations;
        std::shared_ptr<Shader> m_shader = nullptr;

        std::vector<std::shared_ptr<Buffer>> m_buffers;

        int m_animation_state = 0;

        bool m_animated = false;
    };

}