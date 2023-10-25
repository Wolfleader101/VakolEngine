#include "Camera/Camera.hpp"
#include "Math/Math.hpp"

#include <catch2/catch_all.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using namespace Vakol;

TEST_CASE("Camera - Position and Movement", "[Camera]")
{
    Camera camera;

    SECTION("Default camera position")
    {
        REQUIRE_THAT(camera.GetPos().x, Catch::Matchers::WithinRel(0.0f));
        REQUIRE_THAT(camera.GetPos().y, Catch::Matchers::WithinRel(0.0f));
        REQUIRE_THAT(camera.GetPos().z, Catch::Matchers::WithinRel(2.0f));
    }

    SECTION("Set camera position")
    {
        camera.SetPos(1.0f, 2.0f, 3.0f);
        REQUIRE_THAT(camera.GetPos().x, Catch::Matchers::WithinRel(1.0f));
        REQUIRE_THAT(camera.GetPos().y, Catch::Matchers::WithinRel(2.0f));
        REQUIRE_THAT(camera.GetPos().z, Catch::Matchers::WithinRel(3.0f));
    }
}

TEST_CASE("Camera - Direction", "[Camera]")
{
    Camera camera;

    SECTION("Default camera forward direction")
    {
        REQUIRE_THAT(camera.GetForward().x, Catch::Matchers::WithinRel(0.0f));
        REQUIRE_THAT(camera.GetForward().y, Catch::Matchers::WithinRel(0.0f));
        REQUIRE_THAT(camera.GetForward().z, Catch::Matchers::WithinRel(-1.0f));
    }

    SECTION("Default camera right direction")
    {
        REQUIRE_THAT(camera.GetRight().x, Catch::Matchers::WithinRel(0.0f));
        REQUIRE_THAT(camera.GetRight().y, Catch::Matchers::WithinRel(0.0f));
        REQUIRE_THAT(camera.GetRight().z, Catch::Matchers::WithinRel(0.0f));
    }
}

TEST_CASE("Camera - Pitch and Yaw", "[Camera]")
{
    Camera camera;

    SECTION("Default pitch and yaw")
    {
        REQUIRE_THAT(camera.GetPitch(), Catch::Matchers::WithinRel(0.0f));
        REQUIRE_THAT(camera.GetYaw(), Catch::Matchers::WithinRel(-90.0f));
    }

    SECTION("Set pitch and yaw")
    {
        camera.SetPitch(45.0f);
        camera.SetYaw(45.0f);
        REQUIRE_THAT(camera.GetPitch(), Catch::Matchers::WithinRel(45.0f));
        REQUIRE_THAT(camera.GetYaw(), Catch::Matchers::WithinRel(45.0f));
    }
}

TEST_CASE("Camera - Aspect Ratio", "[Camera]")
{
    Camera camera;

    SECTION("Default aspect ratio")
    {
        REQUIRE_THAT(camera.GetAspect(), Catch::Matchers::WithinRel(1.0f));
    }

    SECTION("Set aspect ratio")
    {
        camera.SetAspect(1.7778f); // 16:9 aspect ratio
        REQUIRE_THAT(camera.GetAspect(), Catch::Matchers::WithinRel(1.7778f));
    }
}

TEST_CASE("Camera - Projection and View Matrix", "[Camera]")
{
    Camera camera;

    SECTION("Default projection matrix")
    {
        REQUIRE(camera.GetProjectionMatrix() == Math::Mat4(1.0f)); // Assuming default is identity
    }

    SECTION("Default view matrix")
    {
        REQUIRE(camera.GetViewMatrix() == Math::Mat4(1.0f)); // Assuming default is identity
    }
}

TEST_CASE("Camera - Set Aspect Ratio", "[Camera]")
{
    Camera camera;

    SECTION("Set aspect ratio and check projection matrix")
    {
        camera.SetAspect(1.7778f); // 16:9 aspect ratio
        // You might want to compare with the expected projection matrix here
        // For now, just ensuring it's not the identity matrix
        REQUIRE(camera.GetProjectionMatrix() != Math::Mat4(1.0f));
    }
}

TEST_CASE("Camera - Update", "[Camera]")
{
    Camera camera;

    SECTION("Update camera and check view matrix and directions")
    {
        camera.Update();

        auto forward = camera.GetForward();
        INFO("Actual forward direction: (" << forward.x << ", " << forward.y << ", " << forward.z << ")");

        auto right = camera.GetRight();
        INFO("Actual right direction: (" << right.x << ", " << right.y << ", " << right.z << ")");

        REQUIRE_THAT(forward.x, Catch::Matchers::WithinAbs(0.0f, 1e-5f));
        REQUIRE_THAT(forward.y, Catch::Matchers::WithinAbs(0.0f, 1e-5f));
        REQUIRE_THAT(forward.z, Catch::Matchers::WithinAbs(-1.0f, 1e-5f));

        REQUIRE_THAT(right.x, Catch::Matchers::WithinAbs(1.0f, 1e-5f));
        REQUIRE_THAT(right.y, Catch::Matchers::WithinAbs(0.0f, 1e-5f));
        REQUIRE_THAT(right.z, Catch::Matchers::WithinAbs(0.0f, 1e-5f));

        REQUIRE(camera.GetViewMatrix() != Math::Mat4(1.0f));
    }
}
