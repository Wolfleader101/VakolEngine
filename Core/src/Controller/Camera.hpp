#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

constexpr unsigned char PROJECTION_MATRIX = 0x0;
constexpr unsigned char VIEW_MATRIX = 0x1;

namespace Vakol::Controller
{
    class Camera
	{
    public:
        Camera() = default;
        explicit Camera(const glm::vec3& position);

        const glm::mat4& GetMatrix(unsigned char type) const;

        void SetAspect(float _aspect);

        void Update();

        const glm::vec3& GetPos() const { return position; }

        void SetPos(const float x, const float y, const float z) { position = glm::vec3(x, y, z); }

        const glm::vec3& GetForward() const { return forward; }
        const glm::vec3& GetRight() const { return right; }

    	float GetPitch() const { return pitch; }
    	float GetYaw() const { return yaw; }

        void SetPitch(const float _pitch) { pitch = _pitch; }
        void SetYaw(const float _yaw) { yaw = _yaw; }

    private:
        float pitch = 0.0f;
        float yaw = -90.0f;

        float fov = 45.0f;
        float aspect = 1.3f;
        float near = 0.01f;
        float far = 500.0f;

        glm::mat4 PROJECTION = glm::mat4(1.0f);
        glm::mat4 VIEW = glm::mat4(1.0f);

        glm::vec3 position = glm::vec3(0.0f, 0.0f, 2.0f);
        glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

        glm::vec3 right = glm::vec3(0.0f);

        bool isSprinting = false;

        float forwardDir = 0.0f;
        float rightDir = 0.0f;
    };
}  // namespace Vakol::Controller