#include <ECS/Entity.hpp>
#include <ECS/EntityList.hpp>

#include <catch2/catch_all.hpp>

TEST_CASE("CreateEntity", "[EntityList]")
{
    Vakol::EntityList entityList;
    Vakol::Entity entity = entityList.CreateEntity();
    REQUIRE(entityList.CheckEntityExistence(entity.GetHandle()));
}
// ^
// hard to avoid testing one without the other so there is a little assumption here that it works.
// v
TEST_CASE("Check Entity Existence", "[EntityList]")
{
    Vakol::EntityList entityList;
    Vakol::Entity entity = entityList.CreateEntity();
    REQUIRE(entityList.CheckEntityExistence(entity.GetHandle()));
}

TEST_CASE("Create Entity With Suggested Handle", "[EntityList]")
{
    Vakol::EntityList entityList;
    Vakol::Entity entity = entityList.CreateEntity(13);
    REQUIRE((entity.GetHandle() == 13 && entityList.CheckEntityExistence(13)));
}

TEST_CASE("GetEntity", "[EntityList]")
{
    Vakol::EntityList entityList;
    Vakol::Entity entity = entityList.CreateEntity();
    REQUIRE(entityList.GetEntity(entity.GetHandle()).GetHandle() == entity.GetHandle());
}

TEST_CASE("RemoveEntity", "[EntityList]")
{
    Vakol::EntityList entityList;
    Vakol::Entity entity = entityList.CreateEntity();
    entityList.RemoveEntity(entity.GetHandle());
    REQUIRE_FALSE(entityList.CheckEntityExistence(entity.GetHandle()));
}

TEST_CASE("GetEntityVec", "[EntityList]")
{
    Vakol::EntityList entityList;
    for (int i = 0; i < 37; i++)
    {
        entityList.CreateEntity();
    }
    REQUIRE(entityList.GetEntityVec().size() == 37);
}

TEST_CASE("Clear", "[EntityList]")
{
    Vakol::EntityList entityList;
    entityList.CreateEntity();
    entityList.CreateEntity();
    REQUIRE(entityList.GetEntityVec().size() == 2);
    entityList.Clear();
    REQUIRE(entityList.GetEntityVec().size() == 0);
}
