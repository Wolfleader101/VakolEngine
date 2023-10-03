#include <catch2/catch_all.hpp>

#include <Scripting/ScriptEngine.hpp>

TEST_CASE("Script Engine Tests", "[ScriptEngine]")
{
    Vakol::ScriptEngine scriptEngine;

    SECTION("Test Getting/Setting Global Variables")
    {
        scriptEngine.SetGlobalVariable("test", 1);
        Vakol::LuaType var = scriptEngine.GetGlobalVariable("test");

        REQUIRE(var.as<int>() == 1);
    }

    SECTION("Test Creating Tables")
    {
        Vakol::LuaTable table = scriptEngine.CreateTable();
        table["abc"] = 123;
        REQUIRE(table["abc"] == 123);
    }

    SECTION("Test Creating Scripts")
    {
        Vakol::LuaScript script = scriptEngine.CreateScript("testScripts/test.lua");
        REQUIRE(script.path == "test.lua");

        scriptEngine.SetGlobalVariable("test", 1);
        REQUIRE(script.env["test"] == 1);
    }
}