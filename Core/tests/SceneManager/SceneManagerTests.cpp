#include "SceneManager/Scene.hpp"
#include "SceneManager/SceneManager.hpp"

#include <catch2/catch_all.hpp>

using namespace Vakol;

TEST_CASE("SceneManager - Create and Get Scene", "[SceneManager]")
{
    ScriptEngine mockScriptEngine;
    PhysicsEngine mockPhysicsEngine;

    SceneManager sceneManager(mockScriptEngine, mockPhysicsEngine);

    SECTION("Create a new scene and retrieve it")
    {
        sceneManager.CreateScene("TestScene", "TestScript");
        REQUIRE_NOTHROW(sceneManager.GetScene("TestScene"));
    }

    SECTION("Attempt to retrieve a non-existent scene")
    {
        REQUIRE_THROWS_AS(sceneManager.GetScene("NonExistentScene"), std::runtime_error);
    }

    SECTION("Create a scene with a name that already exists")
    {
        sceneManager.CreateScene("TestScene", "TestScript");
        REQUIRE_THROWS_AS(sceneManager.CreateScene("TestScene", "TestScript"), std::runtime_error);
    }
}

TEST_CASE("SceneManager - Validity", "[SceneManager]")
{
    ScriptEngine mockScriptEngine;
    PhysicsEngine mockPhysicsEngine;

    SceneManager sceneManager(mockScriptEngine, mockPhysicsEngine);

    SECTION("Check validity before creating a scene")
    {
        REQUIRE_FALSE(sceneManager.IsValid());
    }

    SECTION("Check validity after creating a scene")
    {
        sceneManager.CreateScene("TestScene", "TestScript");
        REQUIRE(sceneManager.IsValid());
    }

    SECTION("Check validity after removing a scene")
    {
        sceneManager.CreateScene("TestScene", "TestScript");
        sceneManager.RemoveScene("TestScene");
        REQUIRE_FALSE(sceneManager.IsValid());
    }
}

TEST_CASE("SceneManager - Change Active Scene", "[SceneManager]")
{
    ScriptEngine mockScriptEngine;
    PhysicsEngine mockPhysicsEngine;

    SceneManager sceneManager(mockScriptEngine, mockPhysicsEngine);

    sceneManager.CreateScene("Scene1", "Script1");
    sceneManager.CreateScene("Scene2", "Script2");

    SECTION("Change to an existing scene")
    {
        sceneManager.ChangeActiveScene("Scene2");
        sceneManager.Update();
        REQUIRE_NOTHROW(sceneManager.GetActiveScene());

        REQUIRE(sceneManager.GetActiveScene().getName() == "Scene2");
    }

    SECTION("Change to a non-existent scene")
    {
        REQUIRE_THROWS_AS(sceneManager.ChangeActiveScene("NonExistentScene"), std::runtime_error);
    }
}

TEST_CASE("SceneManager - Get Active Scene", "[SceneManager]")
{
    ScriptEngine mockScriptEngine;
    PhysicsEngine mockPhysicsEngine;

    SceneManager sceneManager(mockScriptEngine, mockPhysicsEngine);

    SECTION("Get active scene when no scenes exist")
    {
        REQUIRE_THROWS_AS(sceneManager.GetActiveScene(), std::runtime_error);
    }

    SECTION("Get active scene when scenes exist")
    {
        sceneManager.CreateScene("Scene1", "Script1");
        sceneManager.CreateScene("Scene2", "Script2");

        REQUIRE_NOTHROW(sceneManager.GetActiveScene());
        REQUIRE(sceneManager.GetActiveScene().getName() == "Scene1");
    }
}

TEST_CASE("SceneManager - Remove Scene", "[SceneManager]")
{
    ScriptEngine mockScriptEngine;
    PhysicsEngine mockPhysicsEngine;

    SceneManager sceneManager(mockScriptEngine, mockPhysicsEngine);

    SECTION("Remove a scene that exists")
    {
        sceneManager.CreateScene("Scene1", "Script1");
        REQUIRE_NOTHROW(sceneManager.RemoveScene("Scene1"));
    }

    SECTION("Remove a scene that does not exist")
    {
        REQUIRE_THROWS_AS(sceneManager.RemoveScene("NonExistentScene"), std::runtime_error);
    }
}

TEST_CASE("SceneManager - Update", "[SceneManager]")
{
    ScriptEngine mockScriptEngine;
    PhysicsEngine mockPhysicsEngine;

    SceneManager sceneManager(mockScriptEngine, mockPhysicsEngine);

    SECTION("Update when no scenes exist")
    {
        REQUIRE_THROWS_AS(sceneManager.Update(), std::runtime_error);
    }

    SECTION("Update when scenes exist")
    {
        sceneManager.CreateScene("Scene1", "Script1");
        sceneManager.CreateScene("Scene2", "Script2");

        REQUIRE_NOTHROW(sceneManager.Update());
    }
}