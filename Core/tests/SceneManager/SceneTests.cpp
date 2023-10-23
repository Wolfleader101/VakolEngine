#include <catch2/catch_all.hpp>

#include <ECS/Components.hpp>
#include <Physics/PhysicsEngine.hpp>
#include <SceneManager/Scene.hpp>
#include <Scripting/ScriptEngine.hpp>
#include <iostream>

TEST_CASE("Scene getName and setName", "[Scene]")
{

    Vakol::PhysicsEngine pe;
    Vakol::PhysicsScene ps = pe.CreateScene();
    Vakol::ScriptEngine se;

    Vakol::LuaScript script = se.CreateScript("test get set name", "Demo/scripts/test/unit_test_scene.lua");

    Vakol::Scene scene("TestScene", script, ps);

    SECTION("getName returns the correct name")
    {
        REQUIRE(scene.getName() == "TestScene");
    }

    SECTION("setName updates the name correctly")
    {
        scene.setName("NewTestScene");
        REQUIRE(scene.getName() == "NewTestScene");
    }
}

TEST_CASE("Scene Entity creation and destruction")
{

    Vakol::PhysicsEngine pe;
    Vakol::ScriptEngine se;

    Vakol::PhysicsScene ps = pe.CreateScene();

    Vakol::LuaScript script = se.CreateScript("test entity", "test/unit_test_scene.lua");

    Vakol::Scene scene("TestScene", script, ps);

    SECTION("CreateEntity creates an entity with the correct tag")
    {
        Vakol::Entity entity = scene.CreateEntity("TestEntity");
        // Replace with the correct way to get the entity's tag
        REQUIRE(entity.GetComponent<Vakol::Components::Tag>().tag == "TestEntity");
    }

    SECTION("DestroyEntity destroys the entity")
    {
        Vakol::Entity entity = scene.CreateEntity("TestEntity");
        scene.DestroyEntity(entity);

        REQUIRE(!scene.GetEntityList().CheckEntityExistence(entity));
    }
}

TEST_CASE("Scene serialization and deserialization", "[Scene]")
{

    Vakol::PhysicsEngine pe;
    Vakol::ScriptEngine se;
    Vakol::PhysicsScene ps = pe.CreateScene();
    Vakol::LuaScript script = se.CreateScript("test serialization", "test/unit_test_scene.lua");

    Vakol::Scene scene("TestScene", script, ps);

    std::string folder = "TestFolder"; // Adjust as necessary

    SECTION("Serialization does not throw exceptions")
    {
        REQUIRE_NOTHROW(scene.Serialize(folder));
    }

    SECTION("Deserialization does not throw exceptions")
    {
        REQUIRE_NOTHROW(scene.Deserialize(folder));
    }
}

TEST_CASE("Scene GetEntityList", "[Scene]")
{
    Vakol::PhysicsEngine pe;
    Vakol::ScriptEngine se;
    Vakol::PhysicsScene ps = pe.CreateScene();
    Vakol::LuaScript script = se.CreateScript("test entity list", "test/unit_test_scene.lua");

    Vakol::Scene scene("TestScene", script, ps);

    SECTION("GetEntityList returns the correct entity list")
    {
        Vakol::EntityList& entityList = scene.GetEntityList();
        REQUIRE(entityList.GetEntityCount() == 0);
    }
}

// I would just be testing getters/setters for the scene class.
// As this class is just a container I think its time better spent on actual modules.