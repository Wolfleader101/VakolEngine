#pragma once

#include <vector>

#include <Model/Assets/Animation/Animation.hpp>

namespace Vakol::Controller
{
	class Animator
	{
	public:
		Animator() = default;
		explicit Animator(std::vector<Model::Assets::Animation>& animations) : m_animations(std::move(animations)) {}

		void Update(const int state, const float delta_time)
		{
			m_animations.at(state).Update(delta_time);
		}

		[[nodiscard]] int nAnimations() const { return static_cast<int>(m_animations.size()); }
		[[nodiscard]] int nTransforms(const int state) const { return m_animations.at(state).numTransforms(); }

		[[nodiscard]] const glm::mat4& transform(const int state) const { return m_animations.at(state).nTransform(0); }
	private:
		std::vector<Model::Assets::Animation> m_animations;
	};
}
