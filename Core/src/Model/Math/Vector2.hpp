#pragma once

#include "Math.hpp"

namespace Vakol::Model::Math {
    struct Vector2 {
        Vector2() = default;

        Vector2(const float x, const float y);
        Vector2(const glm::vec2& vec);

        glm::vec2 vector;
    };

    typedef Vector2 Vec2;
    Vector2 operator+(const Vector2& left, const Vector2& right);

    Vector2 operator-(const Vector2& left, const Vector2& right);

    Vector2 operator*(const Vector2& vec, const float k);

    Vector2 operator/(const Vector2& vec, const float k);

    bool operator==(const Vector2& left, const Vector2& right);

    bool operator!=(const Vector2& left, const Vector2& right);
}  // namespace Vakol::Model::Math
