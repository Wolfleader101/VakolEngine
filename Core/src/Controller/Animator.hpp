#pragma once

#include <Model/Assets/Animation/Animation.hpp>
#include <vector>

namespace Vakol::Controller
{
    /**
     * @brief Class representing an Animator.
     */
    class Animator
    {
      public:
        /**
         * @brief default constructor.
         */
        Animator() = default;

        /**
         * @brief Constructor that takes a vector of animations.
         *
         * @param animations The vector of animations.
         */
        explicit Animator(std::vector<Model::Assets::Animation>& animations) : m_animations(std::move(animations))
        {
        }

        /**
         * @brief Update the animator for a specific animation state.
         *
         * @param state The animation state.
         * @param delta_time The time delta for the update.
         */
        void Update(const int state, const double delta_time)
        {
            if (state > nAnimations())
                return;

            m_animations.at(static_cast<size_t>(state)).Update(delta_time);
        }

        /**
         * @brief Update the animator for all animation states.
         *
         * @param delta_time The time delta for the update.
         */
        void Update(const double delta_time)
        {
            for (size_t i = 0; i < nAnimations(); ++i)
                m_animations.at(i).Update(delta_time);
        }

        /**
         * @brief Get the number of animations.
         *
         * @return int The number of animations.
         */
        int nAnimations() const
        {
            return static_cast<int>(m_animations.size());
        }

        /**
         * @brief Get a constant reference to a specific animation.
         *
         * @param state The animation state.
         * @return const Model::Assets::Animation& The reference to the animation.
         */
        [[nodiscard]] const Model::Assets::Animation& c_get(const int state) const
        {
            return m_animations.at(state);
        }

        /**
         * @brief Get a copy of a specific animation.
         *
         * @param state The animation state.
         * @return Model::Assets::Animation A copy of the animation.
         */
        [[nodiscard]] Model::Assets::Animation get(const int state) const
        {
            return m_animations.at(state);
        }

      private:
        /**
         * @brief Vector storing animations.
         */
        std::vector<Model::Assets::Animation> m_animations;
    };
} // namespace Vakol::Controller
