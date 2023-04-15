#include "Camera.hpp"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Logger.hpp"

constexpr glm::vec3 WORLD_FORWARD = glm::vec3(0.0f, 0.0f, -1.0f);
constexpr glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

namespace Vakol::Controller {

    Camera::Camera(const glm::vec3& position = glm::vec3(0.0f)) : forward(WORLD_FORWARD), up(WORLD_UP) {
        this->position = position;

        // this->Update();
    }

    const glm::mat4 Camera::GetMatrix(const unsigned char type) const {
        switch (type) {
            case PROJECTION_MATRIX:
                return this->PROJECTION;
            case VIEW_MATRIX:
                return this->VIEW;
            case PV_MATRIX:
                return this->PROJECTION * this->VIEW;
            default:
                VK_WARN("Invalid Matrix Type!");
                break;
        }
    }

    void Camera::SetAspect(const float _aspect) { this->aspect = _aspect; }

    void Camera::Update(float deltaTime) {
        glm::vec3 front;

        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        this->forward = glm::normalize(front);
        this->right = glm::normalize(glm::cross(this->forward, WORLD_UP));
        this->up = glm::normalize(glm::cross(this->right, this->forward));

        this->PROJECTION = glm::perspective(glm::radians(this->fov), this->aspect, this->near, this->far);
        this->VIEW = glm::lookAt(position, position + forward, up);
    }
}  // namespace Vakol::Controller