#pragma once

#include <vector>
#include <string>

#include <optional>
#include <map>

#pragma warning(push)
#pragma warning(disable:4201)
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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
		float prev_time = -1;
		int prev_position_index = -1;
		int prev_scale_index = -1;
		int prev_rotation_index = -1;

		glm::mat4 interpolate_frames_at(const float time)
		{
			const auto translation = interpolate_position(time);
			const auto rotation =	  interpolate_rotation(time);
			const auto scale =		   interpolate_scaling(time);

			prev_time = time;

			return translation * rotation * scale;
		}

	private:

		template <typename Key>
		struct KeyTimeCompare
		{
			bool operator()(const Key& lhs, const Key& rhs) const noexcept { return lhs.timestamp < rhs.timestamp; }
			bool operator()(const float time, const Key& rhs) const noexcept { return time < rhs.timestamp; }
			bool operator()(const Key& lhs, const float time) const noexcept { return lhs.timestamp < time; }
		};

		template <typename Key>
		static int GetFrameIndex(const std::vector<Key>& frames, float time, unsigned int start_offset, unsigned int end_offset)
		{
			VK_ASSERT(frames.size() >= 2, "");

			auto itr = std::lower_bound(frames.cbegin() + start_offset, frames.cbegin() + end_offset, time, KeyTimeCompare<Key>{});

			if (itr == frames.cbegin())
				itr = frames.cbegin() + 1;

			VK_ASSERT(itr != frames.cend(), "");
			const auto index = static_cast<int>(std::distance(frames.cbegin(), itr) - 1);

			VK_ASSERT(index >= 0, "");
			VK_ASSERT(index < static_cast<int>(frames.size()) - 1, "");
			VK_ASSERT(frames.at(index).timestamp <= time, "");
			VK_ASSERT(frames.at(index + 1).timestamp >= time, "");

			return index;
		}

		template <typename Key>
		static int UpdateFrameIndex(const std::vector<Key>& frames, const float time, const int prev_index, const float prev_time)
		{
			VK_ASSERT(prev_index < static_cast<int>(frames.size()), "");

			if (prev_index < 0)
				return GetFrameIndex(frames, time, 0, static_cast<unsigned int>(frames.size()));

			VK_ASSERT(prev_index >= 0, "");
			VK_ASSERT(prev_time >= 0, "");

			if (time >= prev_time)
				return GetFrameIndex(frames, time, prev_index, static_cast<unsigned int>(frames.size()));

			return GetFrameIndex(frames, time, 0, prev_index);
		}

		static float GetScaleFactor(const float prev_timestamp, const float next_timestamp, const float time)
		{
			VK_ASSERT(time >= prev_timestamp, "");
			VK_ASSERT(next_timestamp > prev_timestamp, "");

			const auto progress = time - prev_timestamp;
			const auto total = next_timestamp - prev_timestamp;

			VK_ASSERT(progress <= total, "");

			return progress / total;
		}

		[[nodiscard]] glm::mat4 interpolate_position(const float time)
		{
			const int p0 = UpdateFrameIndex(positions, time, prev_position_index, prev_time);

			prev_position_index = p0;

			const auto& [position, timestamp] = positions.at(p0);
			const auto& next = positions.at(p0 + 1);

			const float scale_factor = GetScaleFactor(timestamp, next.timestamp, time);
			const auto& target_position = mix(position, next.position, scale_factor);

			return translate(glm::mat4(1.0f), target_position);
		}

		[[nodiscard]] glm::mat4 interpolate_rotation(const float time)
		{
			const int p0 = UpdateFrameIndex(rotations, time, prev_rotation_index, prev_time);

			prev_rotation_index = p0;

			const auto& [rotation, timestamp] = rotations.at(p0);
			const auto& next = rotations.at(p0 + 1);

			const float scale_factor = GetScaleFactor(timestamp, next.timestamp, time);

			const auto& target_rotation = glm::normalize(glm::slerp(rotation, next.rotation, scale_factor));

			return mat4_cast(target_rotation);
		}

		[[nodiscard]] glm::mat4 interpolate_scaling(const float time)
		{
			const int p0 = UpdateFrameIndex(scales, time, prev_scale_index, prev_time);

			prev_scale_index = p0;

			const auto& [scale, timestamp] = scales.at(p0);
			const auto& next = scales.at(p0 + 1);

			const float scale_factor = GetScaleFactor(timestamp, next.timestamp, time);
			const auto& target_scale = glm::mix(scale, next.scale, scale_factor);

			return glm::scale(glm::mat4(1.0f), target_scale);
		}
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

		void Update(const float delta_time)
		{
			current_time += ticks_per_second * delta_time;
			current_time = fmod(current_time, duration);

			for (int i = 0, count = static_cast<int>(nodes.size()); i < count; ++i)
			{
				auto& [bone, bone_transform, parent, node_transform] = nodes[i];
				VK_ASSERT(i > parent, "");

				const glm::mat4& transform = bone ? bone->interpolate_frames_at(current_time) : node_transform;
				const glm::mat4& parent_transform = parent >= 0 ? nodes[parent].bone_transform : glm::mat4(1.0f);

				bone_transform = parent_transform * transform;

				if (!bone) continue;

				const size_t bone_index = bone->bone_index;
				VK_ASSERT(bone_index < transforms.size(), "\n\nTOO MANY BONES!");

				transforms[bone_index] = global_inverse * parent_transform * bone_transform * bone->offset;
			}
		}

	private:
		glm::mat4 global_inverse;
		std::vector<glm::mat4> transforms;
		std::vector<AnimNode> nodes;

		[[maybe_unused]] unsigned int bone_count = 0;

		float current_time = 0.0f;
		float duration = 0.0f;
		float ticks_per_second = 0.0f;
	};
}
