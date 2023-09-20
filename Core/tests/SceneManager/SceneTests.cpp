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

    Vakol::LuaScript script = se.CreateScript("Demo/scripts/test/unit_test_scene.lua");
    //    replace with proper initialization

    Vakol::Scene scene("TestScene", script, ps);

    // SECTION("getName returns the correct name")
    // {
    //     REQUIRE(scene.getName() == "TestScene");
    // }

    // SECTION("setName updates the name correctly")
    // {
    //     scene.setName("NewTestScene");
    //     REQUIRE(scene.getName() == "NewTestScene");
    // }
}

// TEST_CASE("Scene Entity creation and destruction")
// {

//     Vakol::PhysicsScene ps = pe.CreateScene();

//     Vakol::LuaScript script = se.CreateScript("test/unit_test_scene.lua");

//     Vakol::Scene scene("TestScene", script, ps);

//     SECTION("CreateEntity creates an entity with the correct tag")
//     {
//         Vakol::Entity entity = scene.CreateEntity("TestEntity");
//         // Replace with the correct way to get the entity's tag
//         REQUIRE(entity.GetComponent<Vakol::Components::Tag>().tag == "TestEntity");
//     }

//     SECTION("DestroyEntity destroys the entity")
//     {
//         Vakol::Entity entity = scene.CreateEntity("TestEntity");
//         scene.DestroyEntity(entity);

//         REQUIRE(!scene.GetEntityList().CheckEntityExistence(entity));
//     }
// }

// TEST_CASE("Scene serialization and deserialization", "[Scene]") lmfao
// {

//     Vakol::Scene scene("TestScene", script, ps);

//     std::string folder = "TestFolder"; // Adjust as necessary

//     SECTION("Serialization does not throw exceptions")
//     {
//         REQUIRE_NOTHROW(scene.Serialize(folder));
//     }

//     SECTION("Deserialization does not throw exceptions")
//     {
//         REQUIRE_NOTHROW(scene.Deserialize(folder));
//     }

//     // You might also want to add tests checking the correctness of the serialization/deserialization
// }