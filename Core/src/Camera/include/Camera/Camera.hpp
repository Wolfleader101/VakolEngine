#pragma once

#include <cereal/archives/json.hpp>

#include "Math/Math.hpp"

constexpr unsigned char PROJECTION_MATRIX = 0x0;
constexpr unsigned char VIEW_MATRIX = 0x1;

namespace Vakol
{
    /**
     * @brief Class representing a Camera.
     */
    class Camera
    {
      public:
        Camera() = default;

        /**
         * @brief Constructor that takes a position.
         *
         * @param position The position of the camera.
         */
        explicit Camera(const Math::Vec3& position = Math::Vec3(0.0f, 0.0f, 5.0f));

        /**
         * @brief Get the transformation matrix of the camera based on the specified type.
         *
         * @param type The type of matrix (0 for projection matrix, 1 for view matrix).
         * @return const Math::Mat4& The transformation matrix.
         */
        const Math::Mat4& GetMatrix(unsigned char type) const;

        /**
         * @brief Set the aspect ratio of the camera.
         *
         * @param _aspect The aspect ratio.
         */
        void SetAspect(float _aspect);

        /**
         * @brief Update the camera.
         */
        void Update();

        /**
         * @brief Get the position of the camera.
         *
         * @return const Math::Vec3& The position of the camera.
         */
        const Math::Vec3& GetPos() const
        {
            return position;
        }

        /**
         * @brief Set the position of the camera.
         *
         * @param x The x-coordinate of the position.
         * @param y The y-coordinate of the position.
         * @param z The z-coordinate of the position.
         */
        void SetPos(const float x, const float y, const float z)
        {
            position = Math::Vec3(x, y, z);
        }

        /**
         * @brief Get the forward direction of the camera.
         *
         * @return const Math::Vec3& The forward direction of the camera.
         */
        const Math::Vec3& GetForward() const
        {
            return forward;
        }

        /**
         * @brief Get the right direction of the camera.
         *
         * @return const Math::Vec3& The right direction of the camera.
         */
        const Math::Vec3& GetRight() const
        {
            return right;
        }

        /**
         * @brief Get the pitch angle of the camera.
         *
         * @return float The pitch angle of the camera.
         */
        float GetPitch() const
        {
            return pitch;
        }

        /**
         * @brief Get the yaw angle of the camera.
         *
         * @return float The yaw angle of the camera.
         */
        float GetYaw() const
        {
            return yaw;
        }

        /**
         * @brief Set the pitch angle of the camera.
         *
         * @param _pitch The pitch angle.
         */
        void SetPitch(const float _pitch)
        {
            pitch = _pitch;
        }

        /**
         * @brief Set the yaw angle of the camera.
         *
         * @param _yaw The yaw angle.
         */
        void SetYaw(const float _yaw)
        {
            yaw = _yaw;
        }

        /**
         * @brief Serialize the Camera object using cereal library.
         *
         * @tparam Archive The archive type.
         * @param ar The archive object for serialization.
         */
        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(cereal::make_nvp("position", position), cereal::make_nvp("forward", forward), cereal::make_nvp("up", up),
               cereal::make_nvp("right", right), cereal::make_nvp("pitch", pitch), cereal::make_nvp("yaw", yaw),
               cereal::make_nvp("fov", fov), cereal::make_nvp("aspect", aspect), cereal::make_nvp("near", near),
               cereal::make_nvp("far", far), cereal::make_nvp("projection", PROJECTION), cereal::make_nvp("view", VIEW),
               cereal::make_nvp("forwardDir", forwardDir), cereal::make_nvp("rightDir", rightDir));
        }

      private:
        /**
         * @brief The pitch angle of the camera.
         */
        float pitch = 0.0f;

        /**
         * @brief The yaw angle of the camera.
         */
        float yaw = -90.0f;

        /**
         * @brief The field of view angle of the camera.
         */
        float fov = 65.0f;

        /**
         * @brief The aspect ratio of the camera.
         */
        float aspect = 1.3f;

        /**
         * @brief The near clipping plane distance of the camera.
         */
        float near = 0.01f;

        /**
         * @brief The far clipping plane distance of the camera.
         *
         */
        float far = 1000.0f;

        /**
         * @brief The projection matrix of the camera.
         */
        Math::Mat4 PROJECTION = Math::Mat4(1.0f);

        /**
         * @brief The view matrix of the camera.
         */
        Math::Mat4 VIEW = Math::Mat4(1.0f);

        /**
         * @brief The position of the camera.
         */
        Math::Vec3 position = Math::Vec3(0.0f, 0.0f, 2.0f);

        /**
         * @brief The forward direction of the camera.
         */
        Math::Vec3 forward = Math::Vec3(0.0f, 0.0f, -1.0f);

        /**
         * @brief The up direction of the camera.
         */
        Math::Vec3 up = Math::Vec3(0.0f, 1.0f, 0.0f);

        /**
         * @brief The right direction of the camera.
         */
        Math::Vec3 right = Math::Vec3(0.0f);

        /**
         * @brief Flag indicating whether the camera is sprinting.
         */
        bool isSprinting = false;

        /**
         * @brief The forward direction of the camera (directional input).
         */
        float forwardDir = 0.0f;

        /**
         * @brief The right direction of the camera (directional input).
         */
        float rightDir = 0.0f;
    };
} // namespace Vakol