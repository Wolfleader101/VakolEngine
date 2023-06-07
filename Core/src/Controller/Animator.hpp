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

		void Update(const int state, const float delta_time) { m_animations.at(state).Update(delta_time); }
		
		void Update(const float delta_time) 
		{
			for (int i = 0; i < nAnimations(); ++i)
				m_animations.at(i).Update(delta_time);
		}

		[[nodiscard]] int nAnimations() const { return static_cast<int>(m_animations.size()); }

		[[nodiscard]] const Model::Assets::Animation& c_get(const int state) const { return m_animations.at(state); }
		[[nodiscard]] Model::Assets::Animation get(const int state) const { return m_animations.at(state); }
	private:
		std::vector<Model::Assets::Animation> m_animations;
	};
}
