#include <ECS/Entity.hpp>
#include <ECS/EntityList.hpp>
#include <catch2/catch_all.hpp>

TEST_CASE("Component operations", "[Entity]")
{
    Vakol::EntityList entityList;

    SECTION("AddComponent")
    {
        Vakol::Entity entity = entityList.CreateEntity();

        REQUIRE_FALSE(entity.HasComponent<int>());
        entity.AddComponent<int>(1);
        REQUIRE(entity.HasComponent<int>());
    }

    SECTION("HasComponent")
    {
        Vakol::Entity entity = entityList.CreateEntity();

        entity.AddComponent<int>(1);
        entity.AddComponent<float>(1.0f);
        entity.AddComponent<char>('a');

        REQUIRE(entity.HasComponent<int>());
        REQUIRE(entity.HasComponent<float>());
        REQUIRE(entity.HasComponent<char>());

        REQUIRE(entity.HasComponent<int, float, char>());
        REQUIRE_FALSE(entity.HasComponent<double>());
    }

    SECTION("HasAnyComponent")
    {
        Vakol::Entity entity = entityList.CreateEntity();
        entity.AddComponent<int>(1);
        entity.AddComponent<float>(1.0f);
        entity.AddComponent<char>('a');

        REQUIRE(entity.HasAnyComponent<int, float>());
        REQUIRE(entity.HasAnyComponent<int, float, char>());
        REQUIRE(entity.HasAnyComponent<int, float, char, double>());
    }

    SECTION("RemoveComponent")
    {
        Vakol::Entity entity = entityList.CreateEntity();

        entity.AddComponent<int>(1);
        REQUIRE(entity.HasComponent<int>());
        entity.RemoveComponent<int>();
        REQUIRE_FALSE(entity.HasComponent<int>());
    }

    SECTION("GetComponent")
    {
        Vakol::Entity entity = entityList.CreateEntity();

        entity.AddComponent<int>(1);
        REQUIRE(entity.GetComponent<int>() == 1);
    }
}

TEST_CASE("Entity Handles", "[Entity]")
{
    Vakol::EntityList entityList;

    SECTION("GetHandle")
    {
        Vakol::Entity entity = entityList.CreateEntity();
        REQUIRE(entity.GetHandle() == 0);
    }

    SECTION("SetHandle") // doesn't mean its valid. Just that it can be set
    {
        Vakol::Entity entity = entityList.CreateEntity();
        entity.SetHandle(1);
        REQUIRE(entity.GetHandle() == 1);
    }

    SECTION("operator uint32_t")
    {
        Vakol::Entity entity = entityList.CreateEntity();
        REQUIRE((uint32_t)entity == 0);
    }

    SECTION("operator bool")
    {
        Vakol::Entity entity = entityList.CreateEntity();
        REQUIRE(entity);
    }
}