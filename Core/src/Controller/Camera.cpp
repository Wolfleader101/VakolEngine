#include "Camera.hpp"

#include <GLFW/glfw3.h>

// TODO remove this
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Logger.hpp"

constexpr Vakol::Math::Vec3 WORLD_FORWARD = Vakol::Math::Vec3(0.0f, 0.0f, -1.0f);
constexpr Vakol::Math::Vec3 WORLD_UP = Vakol::Math::Vec3(0.0f, 1.0f, 0.0f);

namespace Vakol::Controller {

    Camera::Camera(const Math::Vec3& position) : forward(WORLD_FORWARD), up(WORLD_UP) { this->position = position; }

    Physics::Frustum Camera::GetFrustum() {
        Physics::Frustum result;
        Math::Mat4 vp = this->VIEW * this->PROJECTION;

        Math::Vec4 col1 = vp[0];  // Column 1
        Math::Vec4 col2 = vp[1];  // Column 2
        Math::Vec4 col3 = vp[2];  // Column 3
        Math::Vec4 col4 = vp[3];  // Column 4

        // Calculating the direction vector for each plane
        result.left.normal = col4 + col1;
        result.right.normal = col4 - col1;
        result.bottom.normal = col4 + col2;
        result.top.normal = col4 - col2;
        result.near.normal = col3;
        result.far.normal = col4 - col3;

        // Calculating the distance from origin for each plane
        result.left.dist = vp[3][3] + vp[0][3];
        result.right.dist = vp[3][3] - vp[0][3];
        result.bottom.dist = vp[3][3] + vp[1][3];
        result.top.dist = vp[3][3] - vp[1][3];
        result.near.dist = vp[2][3];
        result.far.dist = vp[3][3] - vp[2][3];

        for (int i = 0; i < 6; ++i) {
            float mag = 1.0f / Math::Magnitude(result.planes[i].normal);
            result.planes[i].normal = result.planes[i].normal * mag;
            result.planes[i].dist *= mag;
        }
        return result;
    }

    const Math::Mat4& Camera::GetMatrix(const unsigned char type) const {
        switch (type) {
            case PROJECTION_MATRIX:
                return this->PROJECTION;
            case VIEW_MATRIX:
                return this->VIEW;
            default:
                VK_WARN("Invalid Matrix Type!");
                break;
        }
    }

    void Camera::SetAspect(const float _aspect) { this->aspect = _aspect; }

    void Camera::Update() {
        Math::Vec3 front;

        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        this->forward = normalize(front);
        this->right = normalize(cross(this->forward, WORLD_UP));
        this->up = normalize(cross(this->right, this->forward));

        this->PROJECTION = glm::perspective(glm::radians(this->fov), this->aspect, this->near, this->far);
        this->VIEW = glm::lookAt(position, position + forward, up);
    }
}  // namespace Vakol::Controller