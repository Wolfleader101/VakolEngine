#include "Math.hpp"

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

    Mat4 Translation(const Vec3& pos)
    {
        return Mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, pos.x, pos.y, pos.z, 1.0f);
    }

    Mat4 Scale(const Vec3& scale)
    {
        return Mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, scale.x, scale.y, scale.z, 1.0f);
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
        glm::vec4 tempPoint(point, 1.0f);
        tempPoint = mat * tempPoint;
        return Point(tempPoint);
    }

    Vec3 MultiplyVector(const Vec3& vec, const Mat4& mat)
    {
        glm::vec4 temp(vec, 0.0f);
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
} // namespace Vakol::Math