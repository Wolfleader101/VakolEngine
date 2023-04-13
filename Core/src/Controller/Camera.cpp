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
            case _PROJECTION_MATRIX:
                return this->PROJECTION;
            case _VIEW_MATRIX:
                return this->VIEW;
            case _PV_MATRIX:
                return this->PROJECTION * this->VIEW;
            default:
                VK_WARN("Invalid Matrix Type!");
                break;
        }
    }

    void Camera::SetAspect(const float _aspect) { this->aspect = _aspect; }

    void Camera::Update(float deltaTime) {
        float velocity = static_cast<float>((isSprinting ? 50.0f : 30.0f) * deltaTime);

        position += forward * forwardDir * velocity;
        position += right * rightDir * velocity;

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

    void Camera::OnKeyPressed(const int direction) {
        if (direction == GLFW_KEY_W) forwardDir = 1.0f;
        if (direction == GLFW_KEY_S) forwardDir = -1.0f;
        if (direction == GLFW_KEY_A) rightDir = -1.0f;
        if (direction == GLFW_KEY_D) rightDir = 1.0f;

        if (direction == GLFW_KEY_LEFT_SHIFT) isSprinting = true;
    }

    void Camera::OnKeyRelease(const int direction) {
        if (direction == GLFW_KEY_W) forwardDir = 0.0f;
        if (direction == GLFW_KEY_S) forwardDir = 0.0f;
        if (direction == GLFW_KEY_A) rightDir = 0.0f;
        if (direction == GLFW_KEY_D) rightDir = 0.0f;

        if (direction == GLFW_KEY_LEFT_SHIFT) isSprinting = false;
    }

    void Camera::OnMouseMove(float xoffset, float yoffset) {
        xoffset *= 0.05f;  // 0.01f should be replaced with MouseSensitivityX
        yoffset *= 0.05f;  // 0.01f should be replaced with MouseSensitivityY

        this->yaw += xoffset;
        this->pitch += yoffset;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
    }

}  // namespace Vakol::Controller