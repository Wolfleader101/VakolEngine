#pragma once

#include <vector>

#pragma warning(push)
#pragma warning(disable:4201)
#include <glm/gtc/matrix_transform.hpp>
#pragma warning(pop)

#include <optional>

#include <Controller/Logger.hpp>

#include <Model/Assets/Bone.hpp>

namespace Vakol::Model::Assets
{
	struct KeyPosition
	{
		glm::vec3 position{};
		float timestamp = 0.0f;
	};

	struct KeyScale
	{
		glm::vec3 scale{};
		float timestamp = 0.0f;
	};

	struct KeyRotation
	{
		glm::quat rotation{};
		float timestamp = 0.0f;
	};

	struct KeyFrame
	{
		int index = -1;
		glm::mat4 offset = glm::mat4(1.0f); // start off with an identity matrix

		std::vector<KeyPosition> positions;
		std::vector<KeyScale>    scales;
		std::vector<KeyRotation> rotations;

		// Variables to remember the state of the previous frame
		float prev_time = -1;
		int	  prev_position_index = -1;
		int   prev_scale_index = -1;
		int   prev_rotation_index = -1;

		// Interpolates the frames at the given time and returns the resulting transformation matrix
		glm::mat4 interpolate_frames_at(const float time)
		{
			const auto translation = interpolate_position(time);
			const auto rotation = interpolate_rotation(time);
			const auto scale = interpolate_scaling(time);

			prev_time = time;

			return translation * rotation * scale;
		}

	private:
		// Comparator for keyframes based on their timestamps
		template <typename Key>
		struct KeyTimeCompare
		{
			bool operator()(const Key& lhs, const Key& rhs) const noexcept { return lhs.timestamp < rhs.timestamp; }
			bool operator()(const float time, const Key& rhs) const noexcept { return time < rhs.timestamp; }
			bool operator()(const Key& lhs, const float time) const noexcept { return lhs.timestamp < time; }
		};

		// Returns the index of the frame in the vector that corresponds to the given time
		template <typename Key>
		static int GetFrameIndex(const std::vector<Key>& frames, float time, unsigned int start_offset, unsigned int end_offset)
		{
			VK_ASSERT(frames.size() >= 2, "\n\nThere must be at least 2 animation frames"); // Ensure that there are at least 2 frames

			// Find the iterator pointing to the first element with a timestamp not less than the target time
			auto itr = std::lower_bound(frames.cbegin() + start_offset, frames.cbegin() + end_offset, time, KeyTimeCompare<Key>{});

			if (itr == frames.cbegin())
				itr = frames.cbegin() + 1;

			VK_ASSERT(itr != frames.cend(), "\n\nInvalid Iterator!");
			const auto index = static_cast<int>(std::distance(frames.cbegin(), itr)) - 1; // Calculate the index of the frame

			VK_ASSERT(index >= 0, "\n\nIndex must not be non-negative!"); // Ensure that the index is not negative
			VK_ASSERT(index < static_cast<int>(frames.size()) - 1, "\n\nIndex must be within a valid range of animation frames!"); // Ensure that the index is within the valid range of frames

			// Ensure that the frame at the index has a timestamp less than or equal to the target time
			VK_ASSERT(frames.at(index).timestamp <= time, "\n\nFrame must have a timestamp less-than or equal-to the target time!");

			// Ensure that the frame at the next index has a timestamp greater than or equal to the target time
			VK_ASSERT(frames.at(index + 1).timestamp >= time, "\n\nThe next frame must have a timestamp greater-than or equal-to the target time!");

			return index;
		}

		template <typename Key>
		static int UpdateFrameIndex(const std::vector<Key>& frames, const float time, const int prev_index, const float prev_time)
		{
			// Assert that the previous index is within the range of frames
			VK_ASSERT(prev_index < static_cast<int>(frames.size()),
				"\n\nPrevious frame index must be within the valid range of frames!");

			// If no previous index exists, return the frame index based on the entire range of frames
			if (prev_index < 0)
				return GetFrameIndex(frames, time, 0, static_cast<unsigned int>(frames.size()));

			VK_ASSERT(prev_index >= 0, "\n\nPrevious frame index must be non-negative!");
			VK_ASSERT(prev_time >= 0, "\n\nPrevious time frame must be non-negative!");

			// If the current time is greater than or equal to the previous time, return the frame index based on the range from the previous index to the end of frames
			if (time >= prev_time)
				return GetFrameIndex(frames, time, prev_index, static_cast<unsigned int>(frames.size()));

			// Otherwise, return the frame index based on the range from the beginning of frames to the previous index
			return GetFrameIndex(frames, time, 0, prev_index);
		}

		static float GetScaleFactor(const float prev_timestamp, const float next_timestamp, const float time)
		{
			VK_ASSERT(time >= prev_timestamp, "\n\nCurrent time must be greater-than or equal-to the previous keyframe timestamp!"); // Assert that the current time is greater than or equal to the previous timestamp
			VK_ASSERT(next_timestamp > prev_timestamp, "\n\nThe next timestamp must be greater-than or equal-to the previous keyframe timestamp!");  // Assert that the next timestamp is greater than the previous timestamp

			const auto progress = time - prev_timestamp; // Calculate the time progress from the previous timestamp to the current time
			const auto total = next_timestamp - prev_timestamp; // Calculate the total time duration between the previous and next timestamps

			VK_ASSERT(progress <= total, "\n\nProgess must be within the total duration!"); // Assert that the progress is within the total duration

			return progress / total; // Return the scale factor, which represents the relative progress within the total duration
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

			const auto& target_rotation = normalize(slerp(rotation, next.rotation, scale_factor));

			return mat4_cast(target_rotation);
		}

		[[nodiscard]] glm::mat4 interpolate_scaling(const float time)
		{
			const int p0 = UpdateFrameIndex(scales, time, prev_scale_index, prev_time);

			prev_scale_index = p0;

			const auto& [scale, timestamp] = scales.at(p0);
			const auto& next = scales.at(p0 + 1);

			const float scale_factor = GetScaleFactor(timestamp, next.timestamp, time);
			const auto& target_scale = mix(scale, next.scale, scale_factor);

			return glm::scale(glm::mat4(1.0f), target_scale);
		}
	};

	struct AnimNode
	{
		std::optional<KeyFrame> bone;
		glm::mat4 bone_transform{};

		int parent = -1;
		glm::mat4 node_transform{};
	};

	class Animation
	{
	public:
		static constexpr auto MAX_BONE_COUNT = 100;

		Animation() = default;

		Animation(const glm::mat4& root_inverse, std::vector<AnimNode>&& nodes, const unsigned int bone_count, const float duration, const float tps)
			: global_inverse(root_inverse), m_transforms(MAX_BONE_COUNT, glm::mat4(1.0f)), nodes(std::move(nodes)), bone_count(bone_count), duration(duration), ticks_per_second(tps)
		{
			VK_ASSERT(bone_count <= MAX_BONE_COUNT, "\n\nTOO MANY BONES!!");
		}

		void Update(const float delta_time)
		{
			current_time += ticks_per_second * delta_time; // Update the current time based on the ticks per second and delta time
			current_time = fmod(current_time, duration); // Wrap the current time within the duration of the animation

			for (int i = 0, count = static_cast<int>(nodes.size()); i < count; ++i)
			{
				auto& [bone, bone_transform, parent, node_transform] = nodes[i];
				VK_ASSERT(i > parent, "\n\nNode index must be greater than parent index!"); // Assert that the node index is greater than its parent index

				const glm::mat4& transform = bone ? bone->interpolate_frames_at(current_time) : node_transform; // Interpolate the transform based on the bone or node animation
				const glm::mat4& parent_transform = parent >= 0 ? nodes[parent].bone_transform : glm::mat4(1.0f); // Get the parent's transform or use the identity matrix if no parent

				bone_transform = parent_transform * transform; // Compute the final bone transform by combining the parent and current transform

				if (!bone) continue; // Skip further processing if the node is not associated with a bone

				const size_t bone_index = bone->index;
				VK_ASSERT(bone_index < m_transforms.size(), "\n\nTOO MANY BONES!");

				m_transforms[bone_index] = global_inverse * bone_transform * bone->offset; // Compute the final bone transform matrix and store it in m_transforms
			}
		}

		[[nodiscard]] const std::vector<glm::mat4>& transforms() const
		{
			return m_transforms;
		}

		[[nodiscard]] const glm::mat4& nTransform(const int i) const
		{
			return m_transforms.at(i);
		}

		[[nodiscard]] auto numTransforms() const -> int
		{
			return static_cast<int>(m_transforms.size());
		}

	private:
		glm::mat4 global_inverse{};
		std::vector<glm::mat4> m_transforms;
		std::vector<AnimNode> nodes;

		[[maybe_unused]] unsigned int bone_count = 0;

		float current_time = 0.0f;
		float duration = 0.0f;
		float ticks_per_second = 0.0f;
	};
}