#include "Camera/Camera.hpp"

#include <GLFW/glfw3.h>

#include "Logger/Logger.hpp"
#include "Math/Math.hpp"

constexpr Vakol::Math::Vec3 WORLD_FORWARD = Vakol::Math::Vec3(0.0f, 0.0f, -1.0f);
constexpr Vakol::Math::Vec3 WORLD_UP = Vakol::Math::Vec3(0.0f, 1.0f, 0.0f);

namespace Vakol
{
    Camera::Camera(const Math::Vec3& position) : forward(WORLD_FORWARD), up(WORLD_UP)
    {
        this->position = position;
    }

    Math::Vec3 Camera::ScreenToWorldPoint(const Math::Vec2& screenSpace) const
    {
        constexpr float depth = 0.0f;

        // Get Normalized Device Coordinates from screen coordinates (NDC)
        const float ndcX = 2.0f * screenSpace.x / static_cast<float>(this->SCREEN_WIDTH) - 1.0f;
        const float ndcY = 1.0f - 2.0f * screenSpace.y / static_cast<float>(this->SCREEN_HEIGHT);

        const float clipX = ndcX * depth;
        const float clipY = ndcY * depth;

        const Math::Mat4 inverseTransform = Math::Inverse(this->PROJECTION * this->VIEW);

        // homosexual coordinates for unprojection
        const Math::Vec4 clipPos(clipX, clipY, depth, 1.0f);

        Math::Vec4 eyePos = inverseTransform * clipPos;

        if (eyePos.w != 0.0f)
            eyePos /= eyePos.w;

        return eyePos;
    }


    const Math::Mat4& Camera::GetProjectionMatrix() const
    {
        return this->PROJECTION;
    }

    const Math::Mat4& Camera::GetViewMatrix() const
    {
        return this->VIEW;
    }

    void Camera::SetAspect(const unsigned int width, const unsigned int height)
    {
        this->SCREEN_WIDTH = width;
        this->SCREEN_HEIGHT = height;

        this->aspect = static_cast<float>(width) / (height != 0 ? static_cast<float>(height) : 1.0f);
        this->PROJECTION = Math::Perspective(Math::DegToRad(this->fov), this->aspect, this->near, this->far);
    }

    void Camera::Update()
    {
        Math::Vec3 front;

        front.x = cos(Math::DegToRad(yaw)) * cos(Math::DegToRad(pitch));
        front.y = sin(Math::DegToRad(pitch));
        front.z = sin(Math::DegToRad(yaw)) * cos(Math::DegToRad(pitch));

        this->forward = normalize(front);
        this->right = normalize(cross(this->forward, WORLD_UP));
        this->up = normalize(cross(this->right, this->forward));

        this->VIEW = lookAt(position, position + forward, up);
    }
} // namespace Vakol