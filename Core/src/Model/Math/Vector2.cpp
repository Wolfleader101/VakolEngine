#include "Vector2.hpp"

namespace Vakol::Model::Math {

    Vector2::Vector2(const float x, const float y) { this->vector = glm::vec2(x, y); }

    Vector2::Vector2(const glm::vec2& vec) { this->vector = vec; }

    Vector2 operator+(const Vector2& left, const Vector2& right) { return Vector2(left.vector + right.vector); }

    Vector2 operator-(const Vector2& left, const Vector2& right) { return Vector2(left.vector - right.vector); }

    Vector2 operator*(const Vector2& vec, const float k) { return Vector2(vec.vector * k); }

    Vector2 operator/(const Vector2& vec, const float k) { return Vector2(vec.vector / k); }

    bool operator==(const Vector2& left, const Vector2& right) { return left.vector == right.vector; }

    bool operator!=(const Vector2& left, const Vector2& right) { return left.vector != right.vector; }
}