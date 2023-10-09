#include "Rendering/RenderCommand.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Generate Command ID", "[RenderCommand]")
{
    const std::string id = Vakol::Rendering::GenerateID();

    REQUIRE_FALSE(id.empty());
}