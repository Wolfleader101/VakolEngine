#pragma once

#include <Model/Assets/Bone.hpp>
#include <Model/Assets/Animation/Keyframe.hpp>

namespace Vakol::Model::Assets
{
	class Animation
	{
	public:
		static constexpr auto MAX_BONE_COUNT = 100;

		Animation() = default;

		Animation(const glm::mat4& root_inverse, std::vector<AnimationNode>&& nodes, const unsigned int bone_count, const float duration, const float tps) : global_inverse(root_inverse), m_transforms(MAX_BONE_COUNT, glm::mat4(1.0f)), m_nodes(std::move(nodes)), bone_count(bone_count), duration(duration), ticks_per_second(tps) { VK_ASSERT(bone_count <= MAX_BONE_COUNT, "\n\nTOO MANY BONES!!"); }

		void Update(const float delta_time)
		{
			current_time += ticks_per_second * delta_time; // Update the current time based on the ticks per second and delta time
			current_time = fmod(current_time, duration); // Wrap the current time within the duration of the animation

			for (int i = 0, count = numNodes(); i < count; ++i)
			{
				auto& [bone, bone_transform, parent, node_transform] = m_nodes.at(i);
				VK_ASSERT(i > parent, "\n\nNode index must be greater than parent index!"); // Assert that the node index is greater than its parent index

				const auto& transform = bone ? bone->interpolate_frames_at(current_time) : node_transform; // Interpolate the transform based on the bone or node animation
				const auto& parent_transform = parent >= 0 ? m_nodes.at(parent).bone_transform : glm::mat4(1.0f); // Get the parent's transform or use the identity matrix if no parent

				bone_transform = parent_transform * transform; // Compute the final bone transform by combining the parent and current transform

				if (!bone) continue; // Skip further processing if the node is not associated with a bone

				const size_t bone_index = bone->index;
				VK_ASSERT(bone_index < m_transforms.size(), "\n\nTOO MANY BONES!");

				m_transforms[bone_index] = global_inverse * bone_transform * bone->offset; // Compute the final bone transform matrix and store it in m_transforms
			}
		}

		[[nodiscard]] const std::vector<glm::mat4>& transforms() const { return m_transforms; }

		[[nodiscard]] const glm::mat4& nTransform(const int i) const { return m_transforms.at(i); }

		[[nodiscard]] auto duration_s() const ->float { return duration * 0.001f; }
		[[nodiscard]] auto duration_ms() const ->float { return duration; }

		[[nodiscard]] auto numNodes() const -> int { return static_cast<int>(m_nodes.size()); }
		[[nodiscard]] auto numTransforms() const -> int { return static_cast<int>(m_transforms.size()); }

	private:
		glm::mat4 global_inverse{};
		std::vector<glm::mat4> m_transforms;
		std::vector<AnimationNode> m_nodes;

		[[maybe_unused]] unsigned int bone_count = 0;

		float current_time = 0.0f;
		float duration = 0.0f;
		float ticks_per_second = 0.0f;
	};
}