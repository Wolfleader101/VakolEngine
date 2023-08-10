#include "Camera.hpp"

#include <GLFW/glfw3.h>

#include "Logger.hpp"
#include <Math/Math.hpp>

constexpr Vakol::Math::Vec3 WORLD_FORWARD = Vakol::Math::Vec3(0.0f, 0.0f, -1.0f);
constexpr Vakol::Math::Vec3 WORLD_UP = Vakol::Math::Vec3(0.0f, 1.0f, 0.0f);

namespace Vakol::Controller
{

    Camera::Camera(const Math::Vec3& position) : forward(WORLD_FORWARD), up(WORLD_UP)
    {
        this->position = position;

        // this->Update();
    }

    const Math::Mat4& Camera::GetMatrix(const unsigned char type) const
    {
        static constexpr Math::Mat4 defaultMatrix = Math::Mat4(1.0f);
        switch (type)
        {
        case PROJECTION_MATRIX:
            return this->PROJECTION;
        case VIEW_MATRIX:
            return this->VIEW;
        default:
            VK_WARN("Invalid Matrix Type!");
            return defaultMatrix;
        }
    }

    void Camera::SetAspect(const float _aspect)
    {
        this->aspect = _aspect;
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

        this->PROJECTION = Math::Perspective(Math::DegToRad(this->fov), this->aspect, this->near, this->far);
        this->VIEW = lookAt(position, position + forward, up);
    }
} // namespace Vakol::Controller