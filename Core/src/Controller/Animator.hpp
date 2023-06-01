#pragma once

#include <vector>
#include <memory>

#include "Model/Shader.hpp"
#include "Model/Assets/Animation/Animation.hpp"

namespace Vakol::Controller
{
	class Animator
	{
	public:
		explicit Animator(std::vector<Model::Assets::Animation> animations) : m_animations(std::move(animations)) {}

		void Update(float delta_time);

		void SetSpeed(const float speed) { m_speed = speed; }

		[[nodiscard]] int nAnimations() const { return static_cast<int>(m_animations.size()); }

		std::shared_ptr<Model::Shader> shader = nullptr;
	private:
		float m_speed = 1.0f;

		std::vector<Model::Assets::Animation> m_animations;
	};
}
