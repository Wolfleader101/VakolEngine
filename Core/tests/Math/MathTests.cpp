#include <catch2/catch_all.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <Math/Math.hpp>

using namespace Vakol::Math;

TEST_CASE("Dot product calculations are correct", "[dot]")
{
    Vec2 vec2a{1.0f, 0.0f};
    Vec2 vec2b{0.0f, 1.0f};
    REQUIRE_THAT(Dot(vec2a, vec2b), Catch::Matchers::WithinAbs(0.0f, 0.000001f));

    Vec3 vec3a{1.0f, 0.0f, 0.0f};
    Vec3 vec3b{0.0f, 1.0f, 0.0f};
    REQUIRE_THAT(Dot(vec3a, vec3b), Catch::Matchers::WithinAbs(0.0f, 0.000001f));
}

TEST_CASE("Conversion between degrees and radians is correct", "[conversion]")
{
    REQUIRE_THAT(DegToRad(180.0f), Catch::Matchers::WithinRel(glm::pi<float>()));
    REQUIRE_THAT(RadToDeg(glm::pi<float>()), Catch::Matchers::WithinRel(180.0f));

    Vec3 vec{180.0f, 90.0f, 45.0f};

    Vec3 rad = DegToRad(vec);
    REQUIRE_THAT(DegToRad(vec.x), Catch::Matchers::WithinRel(glm::pi<float>()));
    REQUIRE_THAT(DegToRad(vec.y), Catch::Matchers::WithinRel(glm::pi<float>() / 2.0f));
    REQUIRE_THAT(DegToRad(vec.z), Catch::Matchers::WithinRel(glm::pi<float>() / 4.0f));
}

TEST_CASE("Magnitude and MagnitudeSq functions work correctly", "[magnitude]")
{
    Vec2 vec2{3.0f, 4.0f};
    REQUIRE_THAT(Magnitude(vec2), Catch::Matchers::WithinAbs(5.0f, 0.000001f));
    REQUIRE_THAT(MagnitudeSq(vec2), Catch::Matchers::WithinAbs(25.0f, 0.000001f));

    Vec3 vec3{1.0f, 2.0f, 2.0f};
    REQUIRE_THAT(Magnitude(vec3), Catch::Matchers::WithinAbs(3.0f, 0.000001f));
    REQUIRE_THAT(MagnitudeSq(vec3), Catch::Matchers::WithinAbs(9.0f, 0.000001f));
}

TEST_CASE("Distance between two points", "[distance]")
{
    Point p1(0, 0, 0);
    Point p2(1, 1, 1);

    REQUIRE(Distance(p1, p2) == Catch::Approx(std::sqrt(3.0f)));
}

TEST_CASE("Normalization of 2D vector", "[normalize]")
{
    Vec2 vec(3.0f, 4.0f);
    Normalize(vec);

    REQUIRE(vec.x == Catch::Approx(0.6f));
    REQUIRE(vec.y == Catch::Approx(0.8f));
}

TEST_CASE("Normalization of 3D vector", "[normalize]")
{
    Vec3 vec(1.0f, 2.0f, 2.0f);
    Normalize(vec);

    float magnitude = std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    REQUIRE(magnitude == Catch::Approx(1.0f));
}

TEST_CASE("Creating normalized 2D vector", "[normalized]")
{
    Vec2 vec(3.0f, 4.0f);
    Vec2 normalizedVec = Normalized(vec);

    REQUIRE(normalizedVec.x == Catch::Approx(0.6f));
    REQUIRE(normalizedVec.y == Catch::Approx(0.8f));
}

TEST_CASE("Creating normalized 3D vector", "[normalized]")
{
    Vec3 vec(1.0f, 2.0f, 2.0f);
    Vec3 normalizedVec = Normalized(vec);

    float magnitude = std::sqrt(normalizedVec.x * normalizedVec.x + normalizedVec.y * normalizedVec.y +
                                normalizedVec.z * normalizedVec.z);
    REQUIRE(magnitude == Catch::Approx(1.0f));
}

TEST_CASE("Cross product of 3D vectors", "[cross]")
{
    Vec3 vecA(1, 0, 0);
    Vec3 vecB(0, 1, 0);

    Vec3 crossProduct = Cross(vecA, vecB);

    REQUIRE(crossProduct.x == Catch::Approx(0.0f));
    REQUIRE(crossProduct.y == Catch::Approx(0.0f));
    REQUIRE(crossProduct.z == Catch::Approx(1.0f));
}

// float Angle(const Vec2& a, const Vec2& b);

TEST_CASE("Angle between vectors", "[angle]")
{
    Vec2 vecA(1, 0);
    Vec2 vecB(0, 1);

    REQUIRE(Angle(vecA, vecB) == Catch::Approx(glm::pi<float>() / 2.0f));

    Vec3 vec3A(1, 0, 0);
    Vec3 vec3B(0, 1, 0);
    REQUIRE(Angle(vec3A, vec3B) == Catch::Approx(glm::pi<float>() / 2.0f));
}

// Projects a 2D vector onto another 2D vector.

TEST_CASE("Projection of 2D vector onto another 2D vector", "[projection]")
{
    Vec2 vecA(1, 0);
    Vec2 vecB(0, 1);

    Vec2 projection = Project(vecA, vecB);

    REQUIRE(projection.x == Catch::Approx(0.0f));
    REQUIRE(projection.y == Catch::Approx(0.0f));
}

TEST_CASE("Perpendicular vector to two vectors", "[perpendicular]")
{
    Vec2 vecA(1, 0);
    Vec2 vecB(0, 1);

    Vec2 perpendicular2D = Perpendicular(vecA, vecB);

    REQUIRE(perpendicular2D.x == Catch::Approx(1.0f)); // It will remain unchanged as it's already perpendicular to vecB
    REQUIRE(perpendicular2D.y == Catch::Approx(0.0f));

    Vec3 vec3A(1, 0, 0);
    Vec3 vec3B(0, 1, 0);

    Vec3 perpendicular3D = Perpendicular(vec3A, vec3B);

    REQUIRE(perpendicular3D.x == Catch::Approx(1.0f));
    REQUIRE(perpendicular3D.y == Catch::Approx(0.0f));
    REQUIRE(perpendicular3D.z == Catch::Approx(0.0f));
}

TEST_CASE("Euler angles from quaternion", "[euler]")
{
    Quat quat(0.0f, 0.0f, 0.0f, 1.0f);

    Vec3 euler = EulerFromQuat(quat);

    REQUIRE(euler.x == Catch::Approx(0.0f));
    REQUIRE(euler.y == Catch::Approx(0.0f));
    REQUIRE(euler.z == Catch::Approx(glm::pi<float>()));
}

TEST_CASE("Cut function works correctly", "[Cut]")
{

    Mat4 mat(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f);

    SECTION("Cutting the first row and first column")
    {
        Mat3 result = Cut(mat, 0, 0);
        REQUIRE(result[0][0] == Catch::Approx(6.0f));
        REQUIRE(result[0][1] == Catch::Approx(7.0f));
        REQUIRE(result[0][2] == Catch::Approx(8.0f));
        REQUIRE(result[1][0] == Catch::Approx(10.0f));
        REQUIRE(result[1][1] == Catch::Approx(11.0f));
        REQUIRE(result[1][2] == Catch::Approx(12.0f));
        REQUIRE(result[2][0] == Catch::Approx(14.0f));
        REQUIRE(result[2][1] == Catch::Approx(15.0f));
        REQUIRE(result[2][2] == Catch::Approx(16.0f));
    }

    SECTION("Cutting the second row and third column")
    {
        Mat3 result = Cut(mat, 1, 2);
        REQUIRE(result[0][0] == Catch::Approx(1.0f));
        REQUIRE(result[0][1] == Catch::Approx(2.0f));
        REQUIRE(result[0][2] == Catch::Approx(4.0f));
        REQUIRE(result[1][0] == Catch::Approx(9.0f));
        REQUIRE(result[1][1] == Catch::Approx(10.0f));
        REQUIRE(result[1][2] == Catch::Approx(12.0f));
        REQUIRE(result[2][0] == Catch::Approx(13.0f));
        REQUIRE(result[2][1] == Catch::Approx(14.0f));
        REQUIRE(result[2][2] == Catch::Approx(16.0f));
    }
}

TEST_CASE("Length of a line", "[length]")
{
    Line line(Point(0, 0, 0), Point(1, 1, 1));

    REQUIRE(Length(line) == Catch::Approx(std::sqrt(3.0f)));
}

TEST_CASE("Length squared of a line", "[length]")
{
    Line line(Point(0, 0, 0), Point(1, 1, 1));

    REQUIRE(LengthSq(line) == Catch::Approx(3.0f));
}

// good enough lmfao