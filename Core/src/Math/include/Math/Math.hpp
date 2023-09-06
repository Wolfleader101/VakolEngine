#pragma once

#include <glm/glm.hpp>

#include <glm/gtc/quaternion.hpp>

namespace Vakol::Math
{
    //! glm usage is currently just here for backwards compatability reasons
    using Vec2 = glm::vec2;

    using Vec3 = glm::vec3;

    using Vec4 = glm::vec4;
    using Point = Vec3;

    using Mat3 = glm::mat3;
    using Mat4 = glm::mat4;
    using Quat = glm::quat;

    float DegToRad(const float deg);
    Vec3 DegToRad(const Vec3& vec);

    float RadToDeg(const float rad);
    Vec3 RadToDeg(const Vec3& vec);

    float Dot(const Vec2& a, const Vec2& b);
    float Dot(const Vec3& a, const Vec3& b);

    float Magnitude(const Vec2& v);
    float Magnitude(const Vec3& v);

    float MagnitudeSq(const Vec2& v);
    float MagnitudeSq(const Vec3& v);

    float Distance(const Point& p1, const Point& p2);

    void Normalize(Vec2& v);
    void Normalize(Vec3& v);
    Vec2 Normalized(const Vec2& v);
    Vec3 Normalized(const Vec3& v);

    Vec3 Cross(const Vec3& a, const Vec3& b);

    float Angle(const Vec2& a, const Vec2& b);
    float Angle(const Vec3& a, const Vec3& b);
    Quat AngleAxis(float angle, const Vec3& axis);

    Vec2 Project(const Vec2& length, const Vec2& direction);
    Vec3 Project(const Vec3& length, const Vec3& direction);
    Vec2 Perpendicular(const Vec2& len, const Vec2& dir);
    Vec3 Perpendicular(const Vec3& len, const Vec3& dir);

    Vec3 EulerFromQuat(const Quat& quat);

    Mat3 Mat3Cast(const Quat& quat);

    Mat3 Transpose(const Mat3& mat);
    Mat4 Translation(const Vec3& pos);
    Mat4 Scale(const Vec3& scale);

    Mat3 Cut(const Mat4& mat, int row, int col);

    // assumes its degrees
    Mat4 XRotation(float angle);
    // assumes its degrees
    Mat4 YRotation(float angle);
    // assumes its degrees
    Mat4 ZRotation(float angle);

    Mat4 Rotation(float pitch, float yaw, float roll);

    Point MultiplyPoint(const Point& point, const Mat4& mat);
    Vec3 MultiplyVector(const Vec3& vec, const Mat4& mat);

    Mat4 Inverse(const Mat4& mat);
    Mat3 Inverse(const Mat3& mat);

    Mat4 Mat4Cast(const Quat& quaternion);

    struct Line
    {
        Point start;
        Point end;

        inline Line()
        {
        }
        inline Line(const Point& s, const Point& e) : start(s), end(e)
        {
        }
    };

    float Length(const Line& line);
    float LengthSq(const Line& line);

    struct Ray
    {
        Point origin;
        Vec3 dir;

        inline Ray() : dir(0.0f, 0.0f, 1.0f)
        {
        }

        inline Ray(const Point& o, const Vec3& d) : origin(o), dir(d)
        {
            NormalizeDirection();
        }
        inline void NormalizeDirection()
        {
            Normalize(dir);
        }
    };

    Ray FromPoints(const Point& from, const Point& to);

    /**
     * @brief Perform linear interpolation between two values.
     *
     * @param a The starting value.
     * @param b The ending value.
     * @param t The interpolation parameter (between 0 and 1).
     * @return float The interpolated value.
     */
    float Lerp(const float a, const float b, const float t);

    /**
     * @brief Calculate the fraction 't' (between 0 and 1) based on the given value.
     *
     * @param a The starting value.
     * @param b The ending value.
     * @param v The value to calculate the fraction for.
     * @return float The calculated fraction.
     */
    float InverseLerp(const float a, const float b, const float v);

    /**
     * @brief Map a value from one input range to a corresponding value in an output range.
     *
     * @param iMin The minimum value of the input range.
     * @param iMax The maximum value of the input range.
     * @param oMin The minimum value of the output range.
     * @param oMax The maximum value of the output range.
     * @param v The value to remap.
     * @return float The remapped value.
     */
    float Remap(const float iMin, const float iMax, const float oMin, const float oMax, const float v);

    Mat4 Perspective(float fovY, float aspect, float zNear, float zFar);
    Mat4 Orthographic(float left, float right, float bottom, float top);

    Mat4 LookAt(const Vec3& eye, const Vec3& center, const Vec3& up);

    const float* AsArray(const Vec2& v);
    const float* AsArray(const Vec3& v);
    const float* AsArray(const Vec4& v);
    const float* AsArray(const Mat3& m);
    const float* AsArray(const Mat4& m);
    const float* AsArray(const Quat& q);

    float Cos(float val);

    Mat4 Translate(const Mat4& mat, const Vec3& vec);

    Mat4 Scale(const Mat4& mat, const Vec3& vec);

    Mat4 Rotate(const Mat4& mat, float angle, const Vec3& axis);

    Quat Slerp(const Quat& x, const Quat& y, float a);

    Quat Normalized(const Quat& x);
} // namespace Vakol::Math

/**
 * @namespace glm
 * @brief Namespace for GLM library.
 */
namespace glm
{

    /**
     * @brief Serialize glm::vec2 for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::vec2& v)
    {
        archive(v.x, v.y);
    }

    /**
     * @brief Serialize glm::vec3 for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::vec3& v)
    {
        archive(v.x, v.y, v.z);
    }

    /**
     * @brief Serialize glm::vec4 for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::vec4& v)
    {
        archive(v.x, v.y, v.z, v.w);
    }

    /**
     * @brief Serialize glm::ivec2 for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::ivec2& v)
    {
        archive(v.x, v.y);
    }

    /**
     * @brief Serialize glm::ivec3 for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::ivec3& v)
    {
        archive(v.x, v.y, v.z);
    }

    /**
     * @brief Serialize glm::ivec4 for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::ivec4& v)
    {
        archive(v.x, v.y, v.z, v.w);
    }

    /**
     * @brief Serialize glm::uvec2 for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::uvec2& v)
    {
        archive(v.x, v.y);
    }

    /**
     * @brief Serialize glm::uvec3 for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::uvec3& v)
    {
        archive(v.x, v.y, v.z);
    }

    /**
     * @brief Serialize glm::uvec4 for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::uvec4& v)
    {
        archive(v.x, v.y, v.z, v.w);
    }

    /**
     * @brief Serialize glm::dvec2 for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::dvec2& v)
    {
        archive(v.x, v.y);
    }

    /**
     * @brief Serialize glm::dvec3 for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::dvec3& v)
    {
        archive(v.x, v.y, v.z);
    }

    /**
     * @brief Serialize glm::dvec4 for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::dvec4& v)
    {
        archive(v.x, v.y, v.z, v.w);
    }

    /**
     * @brief Serialize glm::mat2 for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::mat2& m)
    {
        archive(m[0], m[1]);
    }

    /**
     * @brief Serialize glm::dmat2 for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::dmat2& m)
    {
        archive(m[0], m[1]);
    }

    /**
     * @brief Serialize glm::mat3 for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::mat3& m)
    {
        archive(m[0], m[1], m[2]);
    }

    /**
     * @brief Serialize glm::mat4 for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::mat4& m)
    {
        archive(m[0], m[1], m[2], m[3]);
    }

    /**
     * @brief Serialize glm::dmat4 for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::dmat4& m)
    {
        archive(m[0], m[1], m[2], m[3]);
    }

    /**
     * @brief Serialize glm::quat for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::quat& q)
    {
        archive(q.x, q.y, q.z, q.w);
    }

    /**
     * @brief Serialize glm::dquat for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::dquat& q)
    {
        archive(q.x, q.y, q.z, q.w);
    }

} // namespace glm