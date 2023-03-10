#pragma once

#include "Math.hpp"

namespace Vakol::Model::Math 
{
    struct Vector3 
    {
        Vector3() = default;

        Vector3(const float x, const float y, const float z);

        Vector3(const glm::vec3& vec);

        glm::vec3 vector;
    };

    Vector3 operator+(const Vector3& left, const Vector3& right);

    Vector3 operator-(const Vector3& left, const Vector3& right);

    Vector3 operator*(const Vector3& vec, const float k);

    Vector3 operator/(const Vector3& vec, const float k);

    bool operator==(const Vector3& left, const Vector3& right);

    bool operator!=(const Vector3& left, const Vector3& right);

    typedef Vector3 Vec3;
}
