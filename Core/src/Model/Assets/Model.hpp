#pragma once

#include <memory>

#include "Animation.hpp"
#include "Mesh.hpp"
#include "Model/Shader.hpp"

namespace Vakol::Model::Assets {
    class Model {
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

        void set_shader(const std::string& path) {
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

        int m_animation_state = 0;

        bool m_animated = false;
    };

}