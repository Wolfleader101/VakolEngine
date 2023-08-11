#pragma once

#include <Rendering/Assets/Animation/Keyframe.hpp>
#include <Rendering/Assets/Bone.hpp>

namespace Vakol::Assets
{
    /**
     * @brief Class representing an animation.
     */
    class Animation
    {
      public:
        static constexpr auto MAX_BONE_COUNT = 100; /**< Maximum number of bones in the animation. */

        /**
         * @brief Default constructor for Animation.
         */
        Animation() = default;

        /**
         * @brief Constructs an Animation object with the given parameters.
         *
         * @param root_inverse The root inverse transformation matrix.
         * @param nodes The animation nodes.
         * @param bone_count The number of bones.
         * @param duration The duration of the animation.
         * @param tps The ticks per second of the animation.
         */
        Animation(const Math::Mat4& root_inverse, std::vector<AnimationNode>&& nodes, const unsigned int bone_count,
                  const double duration, const double tps)
            : global_inverse(root_inverse), m_transforms(MAX_BONE_COUNT, Math::Mat4(1.0f)), m_nodes(std::move(nodes)),
              bone_count(bone_count), duration(duration), ticks_per_second(tps)
        {
            if (bone_count > MAX_BONE_COUNT)
            {
                VK_CRITICAL("Animation::Animation - TOO MANY BONES IN ANIMATION");
            }
        }

        /**
         * @brief Updates the animation state based on the delta time.
         *
         * @param delta_time The time elapsed since the last update.
         */
        void Update(const double delta_time)
        {
            current_time +=
                ticks_per_second * delta_time; // Update the current time based on the ticks per second and delta time
            current_time = fmod(current_time, duration); // Wrap the current time within the duration of the animation

            for (int i = 0, count = numNodes(); i < count; ++i)
            {
                auto& [bone, bone_transform, parent, node_transform] = m_nodes.at(i);
                if (i <= parent)
                {
                    VK_CRITICAL("Animation::Update() - Node index must be greater than parent index!");
                    return;
                }

                const auto& transform =
                    bone ? bone->interpolate_frames_at(current_time)
                         : node_transform; // Interpolate the transform based on the bone or node animation
                const auto& parent_transform =
                    parent >= 0
                        ? m_nodes.at(parent).bone_transform
                        : Math::Mat4(1.0f); // Get the parent's transform or use the identity matrix if no parent

                bone_transform =
                    parent_transform *
                    transform; // Compute the final bone transform by combining the parent and current transform

                if (!bone)
                    continue; // Skip further processing if the node is not associated with a bone

                const size_t bone_index = bone->index;
                if (bone_index >= m_transforms.size())
                {
                    VK_CRITICAL("Animation::Update() - Bone index is greater than transform size");
                    return;
                }

                m_transforms[bone_index] =
                    global_inverse * bone_transform *
                    bone->offset; // Compute the final bone transform matrix and store it in m_transforms
            }
        }

        /**
         * @brief Get the bone transforms.
         *
         * @return The vector of bone transforms.
         */
        [[nodiscard]] const std::vector<Math::Mat4>& transforms() const
        {
            return m_transforms;
        }

        /**
         * @brief Get the transform of the animation.
         *
         * @return The transform matrix.
         */
        [[nodiscard]] const Math::Mat4& transform() const
        {
            return m_transforms.at(0);
        }

        /**
         * @brief Get the duration of the animation in seconds.
         *
         * @return The duration in seconds.
         */
        double duration_s() const
        {
            return duration * 0.001;
        }

        /**
         * @brief Get the duration of the animation in milliseconds.
         *
         * @return The duration in milliseconds.
         */
        double duration_ms() const
        {
            return duration;
        }

        /**
         * @brief Get the current time of the animation in milliseconds.
         *
         * @return The current time in milliseconds.
         */
        double get_current_time_ms() const
        {
            return current_time;
        }

        /**
         * @brief Get the current time of the animation in seconds.
         *
         * @return The current time in seconds.
         */
        double get_current_time_s() const
        {
            return current_time * 0.001f;
        }

        /**
         * @brief Reset the animation to the beginning.
         */
        void reset_animation()
        {
            current_time = 0.0f;
        }

        /**
         * @brief Get the number of animation nodes.
         *
         * @return The number of animation nodes.
         */
        int numNodes() const
        {
            return static_cast<int>(m_nodes.size());
        }

        /**
         * @brief Get the number of bone transforms.
         *
         * @return The number of bone transforms.
         */
        int numTransforms() const
        {
            return static_cast<int>(m_transforms.size());
        }

      private:
        Math::Mat4 global_inverse{};
        std::vector<Math::Mat4> m_transforms;
        std::vector<AnimationNode> m_nodes;

        [[maybe_unused]] unsigned int bone_count = 0;

        double current_time = 0.0f;
        double duration = 0.0f;
        double ticks_per_second = 0.0f;
    };
} // namespace Vakol::Assets