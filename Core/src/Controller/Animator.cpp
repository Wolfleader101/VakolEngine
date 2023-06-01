#include "Animator.hpp"

#pragma warning(push)
#pragma warning(disable:4201)
#include <glm/gtc/type_ptr.hpp>
#pragma warning(pop)

namespace Vakol::Controller
{
	void Animator::Update(const float delta_time)
	{
		for (int i = 0; i < nAnimations(); ++i)
		{
			m_animations.at(i).Update(delta_time);
			//shader->SetMat4v("BONE_TRANSFORMS", nAnimations(), value_ptr(m_animations.at(i).transforms().at(0)));
		}
	}
}