#pragma once

#include <Controller/Logger.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <optional>
#include <vector>

#include "glm/gtc/quaternion.hpp"

namespace Vakol::Model::Assets
{
    /**
     * @brief Struct representing a key position in an animation.
     */
    struct KeyPosition
    {
        glm::vec3 position{};    /**< The position value. */
        double timestamp = 0.0f; /**< The timestamp of the key position. */
    };

    /**
     * @brief Struct representing a key scale in an animation.
     */
    struct KeyScale
    {
        glm::vec3 scale{};       /**< The scale value. */
        double timestamp = 0.0f; /**< The timestamp of the key scale. */
    };

    /**
     * @brief Struct representing a key rotation in an animation.
     */
    struct KeyRotation
    {
        glm::quat rotation{};    /**< The rotation value. */
        double timestamp = 0.0f; /**< The timestamp of the key rotation. */
    };

    /**
     * @brief Struct representing a key frame in an animation.
     */
    struct KeyFrame
    {
        int index = -1;                     /**< The index of the key frame. */
        glm::mat4 offset = glm::mat4(1.0f); /**< The offset matrix of the key frame. */

        std::vector<KeyPosition> positions; /**< The key positions of the key frame. */
        std::vector<KeyScale> scales;       /**< The key scales of the key frame. */
        std::vector<KeyRotation> rotations; /**< The key rotations of the key frame. */

        double prev_time = -1;        /**< The previous time frame. */
        int prev_position_index = -1; /**< The index of the previous key position. */
        int prev_scale_index = -1;    /**< The index of the previous key scale. */
        int prev_rotation_index = -1; /**< The index of the previous key rotation. */

        /**
         * @brief Interpolates the frames at the given time and returns the resulting transformation matrix.
         *
         * @param time The time to interpolate the frames at.
         * @return The interpolated transformation matrix.
         */
        glm::mat4 interpolate_frames_at(const double time)
        {
            const glm::mat4 translation = interpolate_position(time);
            const glm::mat4 rotation = interpolate_rotation(time);
            const glm::mat4 scale = interpolate_scaling(time);

            prev_time = time;

            return translation * rotation * scale;
        }

      private:
        /**
         * @brief Comparator for key frames based on their timestamps.
         * @tparam Key The type of key frame.
         */
        template <typename Key>
        struct KeyTimeCompare
        {
            /**
             * @brief Compare two key frames based on their timestamps.
             *
             * @param lhs The left-hand side key frame.
             * @param rhs The right-hand side key frame.
             * @return True if lhs.timestamp < rhs.timestamp, false otherwise.
             */
            bool operator()(const Key& lhs, const Key& rhs) const noexcept
            {
                return lhs.timestamp < rhs.timestamp;
            }

            /**
             * @brief Compare a key frame timestamp with a double value.
             *
             * @param time The double value representing time.
             * @param rhs The key frame to compare with.
             * @return True if time < rhs.timestamp, false otherwise.
             */
            bool operator()(const double time, const Key& rhs) const noexcept
            {
                return time < rhs.timestamp;
            }

            /**
             * @brief Compare a key frame with a double value representing time.
             *
             * @param lhs The key frame to compare.
             * @param time The double value representing time.
             * @return True if lhs.timestamp < time, false otherwise.
             */
            bool operator()(const Key& lhs, const double time) const noexcept
            {
                return lhs.timestamp < time;
            }
        };

        /**
         * @brief Returns the index of the frame in the vector that corresponds to the given time.
         *
         * @tparam Key The type of key frame.
         * @param frames The vector of key frames.
         * @param time The time to search for.
         * @param start_offset The starting offset for the search.
         * @param end_offset The ending offset for the search.
         * @return The index of the frame if found, -1 otherwise.
         */
        template <typename Key>
        static int GetFrameIndex(const std::vector<Key>& frames, double time, unsigned int start_offset,
                                 unsigned int end_offset)
        {
            if (frames.size() < 2)
            {
                VK_CRITICAL("KeyFrame::GetFrameIndex() - There must be at least 2 animation frames");
                return -1;
            }

            // Find the iterator pointing to the first element with a timestamp not less than the target time
            auto itr = std::lower_bound(frames.cbegin() + start_offset, frames.cbegin() + end_offset, time,
                                        KeyTimeCompare<Key>{});

            if (itr == frames.cbegin())
                itr = frames.cbegin() + 1;

            if (itr == frames.cend())
            {
                VK_CRITICAL("KeyFrame::GetFrameIndex() - Invalid Iterator!");
                return -1;
            }
            const auto index =
                static_cast<int>(std::distance(frames.cbegin(), itr)) - 1; // Calculate the index of the frame

            // Ensure that the index is not negative
            if (index < 0)
            {
                VK_CRITICAL("KeyFrame::GetFrameIndex() - Index must not be negative!");
                return -1;
            }
            // Ensure that the index is within the valid range of frames
            if (index >= static_cast<int>(frames.size()) - 1)
            {
                VK_CRITICAL("KeyFrame::GetFrameIndex() - Index must be within a valid range of animation frames!");
                return -1;
            }

            // Ensure that the frame at the index has a timestamp less than or equal to the target time
            if (frames.at(index).timestamp > time)
            {
                VK_CRITICAL(
                    "KeyFrame::GetFrameIndex() - Frame must have a timestamp less than or equal to the target time!");
                return -1;
            }

            // Ensure that the frame at the next index has a timestamp greater than or equal to the target time
            if (frames.at(index + 1).timestamp < time)
            {
                VK_CRITICAL(
                    "KeyFrame::GetFrameIndex() - The next frame must have a timestamp greater than or equal to the "
                    "target time!");
                return -1;
            }

            return index;
        }

        /**
         * @brief Updates the frame index based on the previous index and time.
         *
         * @tparam Key The type of key frame.
         * @param frames The vector of key frames.
         * @param time The current time.
         * @param prev_index The previous frame index.
         * @param prev_time The previous time frame.
         * @return The updated frame index.
         */
        template <typename Key>
        static int UpdateFrameIndex(const std::vector<Key>& frames, const double time, const int prev_index,
                                    const double prev_time)
        {
            // Assert that the previous index is within the range of frames
            if (prev_index >= static_cast<int>(frames.size()))
            {
                VK_CRITICAL(
                    "KeyFrame::UpdateFrameIndex() - Previous frame index must be within the valid range of frames!");
                return -1;
            }

            // If no previous index exists, return the frame index based on the entire range of frames
            if (prev_index < 0)
                return GetFrameIndex(frames, time, 0, static_cast<unsigned int>(frames.size()));

            if (prev_index < 0)
            {
                VK_CRITICAL("KeyFrame::UpdateFrameIndex() - Previous frame index must be non-negative!");
                return -1;
            }

            if (prev_time < 0)
            {
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

        /**
         * @brief Returns the scale factor based on the previous timestamp, next timestamp, and current time.
         *
         * @param prev_timestamp The previous timestamp.
         * @param next_timestamp The next timestamp.
         * @param time The current time.
         * @return The scale factor.
         */
        static double GetScaleFactor(const double prev_timestamp, const double next_timestamp, const double time)
        {
            // Assert that the current time is greater than or equal to the previous timestamp
            if (time < prev_timestamp)
            {
                VK_CRITICAL(
                    "KeyFrame::GetScaleFactor() - Current time must be greater than or equal to the previous keyframe "
                    "timestamp!");
                return -1.f;
            }

            // Assert that the next timestamp is greater than the previous timestamp
            if (next_timestamp <= prev_timestamp)
            {
                VK_CRITICAL(
                    "KeyFrame::GetScaleFactor() - The next timestamp must be greater than the previous keyframe "
                    "timestamp!");
                return -1.f;
            }

            const auto progress =
                time - prev_timestamp; // Calculate the time progress from the previous timestamp to the current time
            const auto total =
                next_timestamp -
                prev_timestamp; // Calculate the total time duration between the previous and next timestamps

            // Assert that the progress is within the total duration
            if (progress > total)
            {
                VK_CRITICAL("KeyFrame::GetScaleFactor() - Progress must be within the total duration!");
                return -1.f;
            }

            return progress /
                   total; // Return the scale factor, which represents the relative progress within the total duration
        }

        /**
         * @brief Interpolates the position at the given time and returns the resulting transformation matrix.
         *
         * @param time The time to interpolate the position at.
         * @return The interpolated transformation matrix for the position.
         */
        glm::mat4 interpolate_position(const double time)
        {
            const int p0 = UpdateFrameIndex(positions, time, prev_position_index, prev_time);

            prev_position_index = p0;

            const auto& [position, timestamp] = positions.at(p0);
            const auto& next = positions.at(p0 + 1);

            const double scale_factor = GetScaleFactor(timestamp, next.timestamp, time);
            const auto& target_position = mix(position, next.position, scale_factor);

            return translate(glm::mat4(1.0f), target_position);
        }

        /**
         * @brief Interpolates the rotation at the given time and returns the resulting transformation matrix.
         *
         * @param time The time to interpolate the rotation at.
         * @return The interpolated transformation matrix for the rotation.
         */
        glm::mat4 interpolate_rotation(const double time)
        {
            const int p0 = UpdateFrameIndex(rotations, time, prev_rotation_index, prev_time);

            prev_rotation_index = p0;

            const Vakol::Model::Assets::KeyRotation& first = rotations.at(p0);
            const Vakol::Model::Assets::KeyRotation& next = rotations.at(p0 + 1);

            const double scale_factor = GetScaleFactor(first.timestamp, next.timestamp, time);

            glm::quat target_rotation =
                glm::normalize(glm::slerp(first.rotation, next.rotation, static_cast<float>(scale_factor)));

            return mat4_cast(target_rotation);
        }

        /**
         * @brief Interpolates the scaling at the given time and returns the resulting transformation matrix.
         *
         * @param time The time to interpolate the scaling at.
         * @return The interpolated transformation matrix for the scaling.
         */
        glm::mat4 interpolate_scaling(const double time)
        {
            const int p0 = UpdateFrameIndex(scales, time, prev_scale_index, prev_time);

            prev_scale_index = p0;

            const auto& [scale, timestamp] = scales.at(p0);
            const auto& next = scales.at(p0 + 1);

            const double scale_factor = GetScaleFactor(timestamp, next.timestamp, time);
            const auto& target_scale = mix(scale, next.scale, scale_factor);

            return glm::scale(glm::mat4(1.0f), target_scale);
        }
    };

    /**
     * @brief Struct representing an animation node.
     */
    struct AnimationNode
    {
        std::optional<KeyFrame> bone; /**< The optional key frame representing a bone. */
        glm::mat4 bone_transform{};   /**< The bone transformation matrix. */

        int parent = -1;            /**< The index of the parent node. */
        glm::mat4 node_transform{}; /**< The node transformation matrix. */
    };
} // namespace Vakol::Model::Assets
