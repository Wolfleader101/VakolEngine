#include <Model/Math/Vector3.hpp>

namespace Vakol::Model::Math {

    Vector3::Vector3(const float x, const float y, const float z) { this->vector = glm::vec3(x, y, z); }

    Vector3::Vector3(const glm::vec3& vec) { this->vector = vec; }

    Vector3 operator+(const Vector3& left, const Vector3& right) { return Vector3(left.vector + right.vector); }

    Vector3 operator-(const Vector3& left, const Vector3& right) { return Vector3(left.vector - right.vector); }

    Vector3 operator*(const Vector3& vec, const float k) { return Vector3(vec.vector * k); }

    Vector3 operator/(const Vector3& vec, const float k) { return Vector3(vec.vector / k); }

    bool operator==(const Vector3& left, const Vector3& right) { return left.vector == right.vector; }

    bool operator!=(const Vector3& left, const Vector3& right) { return left.vector != right.vector; }
}  // namespace Vakol::Model::Math