#pragma once

#include <vector>
#include <optional>

#include <Controller/Logger.hpp>

#pragma warning(push)
#pragma warning(disable:4201)
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/quaternion.hpp"
#pragma warning(pop)

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

	struct AnimationNode
	{
		std::optional<KeyFrame> bone;
		glm::mat4 bone_transform{};

		int parent = -1;
		glm::mat4 node_transform{};
	};
}