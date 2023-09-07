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

    const Math::Mat4& Camera::GetProjectionMatrix() const
    {
        return this->PROJECTION;
    }

    const Math::Mat4& Camera::GetViewMatrix() const
    {
        return this->VIEW;
    }

    void Camera::SetAspect(const float _aspect)
    {
        this->aspect = _aspect;
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