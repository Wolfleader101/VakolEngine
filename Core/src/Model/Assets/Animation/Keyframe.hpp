#pragma once

#include <Controller/Logger.hpp>
#include <optional>
#include <vector>

#pragma warning(push)
#pragma warning(disable : 4201)
#include <glm/gtc/matrix_transform.hpp>

#include "glm/gtc/quaternion.hpp"
#pragma warning(pop)

namespace Vakol::Model::Assets {
    struct KeyPosition {
        glm::vec3 position{};
        float timestamp = 0.0f;
    };

    struct KeyScale {
        glm::vec3 scale{};
        float timestamp = 0.0f;
    };

    struct KeyRotation {
        glm::quat rotation{};
        float timestamp = 0.0f;
    };

    struct KeyFrame {
        int index = -1;
        glm::mat4 offset = glm::mat4(1.0f);  // start off with an identity matrix

        std::vector<KeyPosition> positions;
        std::vector<KeyScale> scales;
        std::vector<KeyRotation> rotations;

        // Variables to remember the state of the previous frame
        float prev_time = -1;
        int prev_position_index = -1;
        int prev_scale_index = -1;
        int prev_rotation_index = -1;

        // Interpolates the frames at the given time and returns the resulting transformation matrix
        glm::mat4 interpolate_frames_at(const float time) {
            const auto translation = interpolate_position(time);
            const auto rotation = interpolate_rotation(time);
            const auto scale = interpolate_scaling(time);

            prev_time = time;

            return translation * rotation * scale;
        }

       private:
        // Comparator for keyframes based on their timestamps
        template <typename Key>
        struct KeyTimeCompare {
            bool operator()(const Key& lhs, const Key& rhs) const noexcept { return lhs.timestamp < rhs.timestamp; }
            bool operator()(const float time, const Key& rhs) const noexcept { return time < rhs.timestamp; }
            bool operator()(const Key& lhs, const float time) const noexcept { return lhs.timestamp < time; }
        };

        // Returns the index of the frame in the vector that corresponds to the given time
        template <typename Key>
        static int GetFrameIndex(const std::vector<Key>& frames, float time, unsigned int start_offset,
                                 unsigned int end_offset) {
            if (frames.size() < 2) {
                VK_CRITICAL("KeyFrame::GetFrameIndex() - There must be at least 2 animation frames");
                return -1;
            }

            // Find the iterator pointing to the first element with a timestamp not less than the target time
            auto itr = std::lower_bound(frames.cbegin() + start_offset, frames.cbegin() + end_offset, time,
                                        KeyTimeCompare<Key>{});

            if (itr == frames.cbegin()) itr = frames.cbegin() + 1;

            if (itr == frames.cend()) {
                VK_CRITICAL("KeyFrame::GetFrameIndex() - Invalid Iterator!");
                return -1;
            }
            const auto index =
                static_cast<int>(std::distance(frames.cbegin(), itr)) - 1;  // Calculate the index of the frame

            // Ensure that the index is not negative
            if (index < 0) {
                VK_CRITICAL("KeyFrame::GetFrameIndex() - Index must not be negative!");
                return -1;
            }
            // Ensure that the index is within the valid range of frames
            if (index >= static_cast<int>(frames.size()) - 1) {
                VK_CRITICAL("KeyFrame::GetFrameIndex() - Index must be within a valid range of animation frames!");
                return -1;
            }

            // Ensure that the frame at the index has a timestamp less than or equal to the target time
            if (frames.at(index).timestamp > time) {
                VK_CRITICAL(
                    "KeyFrame::GetFrameIndex() - Frame must have a timestamp less than or equal to the target time!");
                return -1;
            }

            // Ensure that the frame at the next index has a timestamp greater than or equal to the target time
            if (frames.at(index + 1).timestamp < time) {
                VK_CRITICAL(
                    "KeyFrame::GetFrameIndex() - The next frame must have a timestamp greater than or equal to the "
                    "target time!");
                return -1;
            }

            return index;
        }

        template <typename Key>
        static int UpdateFrameIndex(const std::vector<Key>& frames, const float time, const int prev_index,
                                    const float prev_time) {
            // Assert that the previous index is within the range of frames
            if (prev_index >= static_cast<int>(frames.size())) {
                VK_CRITICAL(
                    "KeyFrame::UpdateFrameIndex() - Previous frame index must be within the valid range of frames!");
                return -1;
            }

            // If no previous index exists, return the frame index based on the entire range of frames
            if (prev_index < 0) return GetFrameIndex(frames, time, 0, static_cast<unsigned int>(frames.size()));

            if (prev_index < 0) {
                VK_CRITICAL("KeyFrame::UpdateFrameIndex() - Previous frame index must be non-negative!");
                return -1;
            }

            if (prev_time < 0) {
                VK_CRITICAL("KeyFrame::UpdateFrameIndex() - Previous time frame must be non-negative!");
                return -1;
            }

            // If the current time is greater than or equal to the previous time, return the frame index based on the
            // range from the previous index to the end of frames
            if (time >= prev_time)
                return GetFrameIndex(frames, time, prev_index, static_cast<unsigned int>(frames.size()));

            // Otherwise, return the frame index based on the range from the beginning of frames to the previous index
            return GetFrameIndex(frames, time, 0, prev_index);
        }

        static float GetScaleFactor(const float prev_timestamp, const float next_timestamp, const float time) {
            // Assert that the current time is greater than or equal to the previous timestamp
            if (time < prev_timestamp) {
                VK_CRITICAL(
                    "KeyFrame::GetScaleFactor() - Current time must be greater than or equal to the previous keyframe "
                    "timestamp!");
                return -1.f;
            }

            // Assert that the next timestamp is greater than the previous timestamp
            if (next_timestamp <= prev_timestamp) {
                VK_CRITICAL(
                    "KeyFrame::GetScaleFactor() - The next timestamp must be greater than the previous keyframe "
                    "timestamp!");
                return -1.f;
            }

            const auto progress =
                time - prev_timestamp;  // Calculate the time progress from the previous timestamp to the current time
            const auto total =
                next_timestamp -
                prev_timestamp;  // Calculate the total time duration between the previous and next timestamps

            // Assert that the progress is within the total duration
            if (progress > total) {
                VK_CRITICAL("KeyFrame::GetScaleFactor() - Progress must be within the total duration!");
                return -1.f;
            }

            return progress /
                   total;  // Return the scale factor, which represents the relative progress within the total duration
        }

        [[nodiscard]] glm::mat4 interpolate_position(const float time) {
            const int p0 = UpdateFrameIndex(positions, time, prev_position_index, prev_time);

            prev_position_index = p0;

            const auto& [position, timestamp] = positions.at(p0);
            const auto& next = positions.at(p0 + 1);

            const float scale_factor = GetScaleFactor(timestamp, next.timestamp, time);
            const auto& target_position = mix(position, next.position, scale_factor);

            return translate(glm::mat4(1.0f), target_position);
        }

        [[nodiscard]] glm::mat4 interpolate_rotation(const float time) {
            const int p0 = UpdateFrameIndex(rotations, time, prev_rotation_index, prev_time);

            prev_rotation_index = p0;

            const auto& [rotation, timestamp] = rotations.at(p0);
            const auto& next = rotations.at(p0 + 1);

            const float scale_factor = GetScaleFactor(timestamp, next.timestamp, time);

            const auto& target_rotation = normalize(slerp(rotation, next.rotation, scale_factor));

            return mat4_cast(target_rotation);
        }

        [[nodiscard]] glm::mat4 interpolate_scaling(const float time) {
            const int p0 = UpdateFrameIndex(scales, time, prev_scale_index, prev_time);

            prev_scale_index = p0;

            const auto& [scale, timestamp] = scales.at(p0);
            const auto& next = scales.at(p0 + 1);

            const float scale_factor = GetScaleFactor(timestamp, next.timestamp, time);
            const auto& target_scale = mix(scale, next.scale, scale_factor);

            return glm::scale(glm::mat4(1.0f), target_scale);
        }
    };

    struct AnimationNode {
        std::optional<KeyFrame> bone;
        glm::mat4 bone_transform{};

        int parent = -1;
        glm::mat4 node_transform{};
    };
}  // namespace Vakol::Model::Assets
