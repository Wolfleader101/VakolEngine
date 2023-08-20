#include "Math/Math.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace Vakol::Math
{
    float Dot(const Vec2& a, const Vec2& b)
    {
        return a.x * b.x + a.y * b.y;
    }
    float Dot(const Vec3& a, const Vec3& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    float DegToRad(const float deg)
    {
        return glm::radians(deg);
    }

    Vec3 DegToRad(const Vec3& vec)
    {
        return glm::radians(vec);
    }

    float RadToDeg(const float rad)
    {
        return glm::degrees(rad);
    }

    Vec3 RadToDeg(const Vec3& vec)
    {
        return glm::degrees(vec);
    }

    float Magnitude(const Vec2& v)
    {
        return sqrtf(Dot(v, v));
    }
    float Magnitude(const Vec3& v)
    {
        return sqrtf(Dot(v, v));
    }
    float MagnitudeSq(const Vec2& v)
    {
        return Dot(v, v);
    }
    float MagnitudeSq(const Vec3& v)
    {
        return Dot(v, v);
    }

    float Distance(const Point& p1, const Point& p2)
    {
        Vec3 t = p1 - p2;
        return Magnitude(t);
    }

    void Normalize(Vec2& v)
    {
        v = v * (1.0f / Magnitude(v));
    }
    void Normalize(Vec3& v)
    {
        v = v * (1.0f / Magnitude(v));
    }
    Vec2 Normalized(const Vec2& v)
    {
        return v * (1.0f / Magnitude(v));
    }
    Vec3 Normalized(const Vec3& v)
    {
        return v * (1.0f / Magnitude(v));
    }

    Vec3 Cross(const Vec3& a, const Vec3& b)
    {
        Vec3 res;
        res.x = a.y * b.z - a.z * b.y;
        res.y = a.z * b.x - a.x * b.z;
        res.z = a.x * b.y - a.y * b.x;
        return res;
    }

    float Angle(const Vec2& a, const Vec2& b)
    {
        float m = sqrtf(MagnitudeSq(a) * MagnitudeSq(b));
        return acos(Dot(a, b) / m);
    }

    float Angle(const Vec3& a, const Vec3& b)
    {
        float m = sqrtf(MagnitudeSq(a) * MagnitudeSq(b));
        return acos(Dot(a, b) / m);
    }

    Vec2 Project(const Vec2& length, const Vec2& direction)
    {
        float dot = Dot(length, direction);
        float magSq = MagnitudeSq(direction);
        return direction * (dot / magSq);
    }

    Vec3 Project(const Vec3& length, const Vec3& direction)
    {
        float dot = Dot(length, direction);
        float magSq = MagnitudeSq(direction);
        return direction * (dot / magSq);
    }

    Vec2 Perpendicular(const Vec2& len, const Vec2& dir)
    {
        return len - Project(len, dir);
    }
    Vec3 Perpendicular(const Vec3& len, const Vec3& dir)
    {
        return len - Project(len, dir);
    }

    float Length(const Line& line)
    {
        return Magnitude(line.start - line.end);
    }
    float LengthSq(const Line& line)
    {
        return MagnitudeSq(line.start - line.end);
    }

    Ray FromPoints(const Point& from, const Point& to)
    {
        return Ray(from, Normalized(to - from));
    }

    Mat4 Mat4Cast(const Quat& quaternion)
    {
        return glm::mat4_cast(quaternion);
    }

    Mat4 Translation(const Vec3& pos)
    {
        return Mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, pos.x, pos.y, pos.z, 1.0f);
    }

    Mat4 Scale(const Vec3& scale)
    {
        return Mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, scale.x, scale.y, scale.z,
                    1.0f);
    }

    Mat4 XRotation(float angle)
    {
        angle = glm::radians(angle);
        return Mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, cosf(angle), sinf(angle), 0.0f, 0.0f, -sinf(angle), cos(angle), 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f);
    }
    Mat4 YRotation(float angle)
    {
        angle = glm::radians(angle);
        return Mat4(cosf(angle), 0.0f, -sinf(angle), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, sinf(angle), 0.0f, cosf(angle), 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f);
    }

    Mat4 ZRotation(float angle)
    {
        angle = glm::radians(angle);
        return Mat4(cosf(angle), sinf(angle), 0.0f, 0.0f, -sinf(angle), cosf(angle), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f);
    }

    Mat4 Rotation(float pitch, float yaw, float roll)
    {
        return ZRotation(roll) * XRotation(pitch) * YRotation(yaw);
    }

    Point MultiplyPoint(const Point& point, const Mat4& mat)
    {
        Math::Vec4 tempPoint(point, 1.0f);
        tempPoint = mat * tempPoint;
        return Point(tempPoint);
    }

    Vec3 MultiplyVector(const Vec3& vec, const Mat4& mat)
    {
        Math::Vec4 temp(vec, 0.0f);
        temp = mat * temp;
        return Vec3(temp);
    }

    Mat3 Cut(const Mat4& mat, int row, int col)
    {
        Mat3 res;
        int targetRow = 0;
        for (int i = 0; i < 4; ++i)
        {
            if (i == row)
                continue;
            int targetCol = 0;
            for (int j = 0; j < 4; ++j)
            {
                if (j == col)
                    continue;
                res[targetRow][targetCol] = mat[i][j];
                ++targetCol;
            }
            ++targetRow;
        }
        return res;
    }

    Mat4 Inverse(const Mat4& mat)
    {
        return glm::inverse(mat);
    }

    float Lerp(const float a, const float b, const float t)
    {
        return (1.0f - t) * a + b * t;
    }

    /// return a fraction 't' (between 0 and 1) based on the given value
    float InverseLerp(const float a, const float b, const float v)
    {
        return (v - a) / (b - a);
    }

    /// takes a value within a given input range into a given output range
    float Remap(const float iMin, const float iMax, const float oMin, const float oMax, const float v)
    {
        float t = InverseLerp(iMin, iMax, v);

        return Lerp(oMin, oMax, t);
    }

    Mat4 Perspective(const float fovY, const float aspect, const float zNear, const float zFar)
    {
        return glm::perspective(fovY, aspect, zNear, zFar);
    }

    Mat4 Orthographic(const float left, const float right, const float bottom, const float top)
    {
        return glm::ortho(left, right, bottom, top);
    }

    Mat4 LookAt(const Vec3& eye, const Vec3& center, const Vec3& up)
    {
        return glm::lookAt(eye, center, up);
    }

    const float* AsArray(const Vec2& v)
    {
        return glm::value_ptr(v);
    }
    const float* AsArray(const Vec3& v)
    {
        return glm::value_ptr(v);
    }
    const float* AsArray(const Vec4& v)
    {
        return glm::value_ptr(v);
    }
    const float* AsArray(const Mat3& m)
    {
        return glm::value_ptr(m);
    }
    const float* AsArray(const Mat4& m)
    {
        return glm::value_ptr(m);
    }
    const float* AsArray(const Quat& q)
    {
        return glm::value_ptr(q);
    }

    float Cos(float val)
    {
        return glm::cos(val);
    }

    Mat4 Translate(const Mat4& mat, const Vec3& vec)
    {
        return glm::translate(mat, vec);
    }

    Mat4 Scale(const Mat4& mat, const Vec3& vec)
    {
        return glm::scale(mat, vec);
    }

    Mat4 Rotate(const Mat4& mat, float angle, const Vec3& axis)
    {
        return glm::rotate(mat, angle, axis);
    }

    Quat Slerp(const Quat& x, const Quat& y, float a)
    {
        return glm::slerp(x, y, a);
    }

    Quat Normalized(const Quat& x)
    {
        return glm::normalize(x);
    }

} // namespace Vakol::Math