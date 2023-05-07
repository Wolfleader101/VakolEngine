#pragma once

#include <vector>
#include <string>
#include <span>

#include <optional>
#include <map>

#pragma warning(push)
#pragma warning(disable:4201)
#include <glm/gtc/quaternion.hpp>
#pragma warning(pop)

namespace Vakol::Model::Assets
{
	struct BoneInfo
	{
		int index = -1;

		// Inverse-bind matrix or inverse bind pose matrix or "offset" matrix:
			// https://stackoverflow.com/questions/50143649/what-does-moffsetmatrix-actually-do-in-assimp.
		glm::mat4 offset;
	};

	struct BoneInfoRemap
	{
		std::map<std::string, BoneInfo, std::less<>> name_to_info; // less-than comparison

		int next_bone_id = 0;

		int add_new_bone(std::string&& name, const glm::mat4& bone_to_model)
		{
			auto [itr, inserted] = name_to_info.insert(std::make_pair(std::move(name), BoneInfo{ -1, bone_to_model }));

			VK_ASSERT(inserted, "\n\nInserting duplicate bone.");

			auto& [index, offset] = itr->second;

			index = next_bone_id++;

			return index;
		}

		const BoneInfo* get(const char* name) const
		{
			const auto itr = name_to_info.find(name);

			return itr != name_to_info.end() ? &itr->second : nullptr;
		}
	};

	struct KeyPosition
	{
		glm::vec3 position;
		float timestamp = 0.0f;
	};

	struct KeyScale
	{
		glm::vec3 scale;
		float timestamp = 0.0f;
	};

	struct KeyRotation
	{
		glm::quat rotation;
		float timestamp = 0.0f;
	};

	struct BoneKeyFrame
	{
		int bone_index = -1;
		// offset = model-space to bone-space
		glm::mat4 offset = glm::mat4(1.0f); // start off with an identity matrix

		std::vector<KeyPosition> positions;
		std::vector<KeyScale> scales;
		std::vector<KeyRotation> rotations;

		// remember previous frame's state to find next frame keys quicker
		float prev_time = -1.0f;
		int prev_position_index = -1;
		int prev_scale_index = -1;
		int prev_rotation_index = -1;
	};

	struct AnimNode
	{
		std::optional<BoneKeyFrame> bone;
		glm::mat4 bone_transform{};

		int parent = -1;
		glm::mat4 node_transform{};
	};

	class Animation
	{
	public:
		static constexpr size_t MAX_BONE_COUNT = 206;

		Animation() = default;

		Animation(const glm::mat4& root_inverse, std::vector<AnimNode>&& nodes, const unsigned int bone_count, const float duration, const float tps)
			: global_inverse(root_inverse), transforms(MAX_BONE_COUNT, glm::mat4(1.0f)), nodes(std::move(nodes)), bone_count(bone_count), duration(duration), ticks_per_second(tps)
		{
			VK_ASSERT(bone_count <= MAX_BONE_COUNT, "\n\nTOO MANY BONES!!");
		}

	private:
		glm::mat4 global_inverse;
		std::vector<glm::mat4> transforms;
		std::vector<AnimNode> nodes;

		unsigned int bone_count = 0;

		float current_time = 0.0f;
		float duration = 0.0f;
		float ticks_per_second = 0.0f;
	};
}
