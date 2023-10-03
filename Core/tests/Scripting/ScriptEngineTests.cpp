#include <catch2/catch_all.hpp>

#include <Scripting/ScriptEngine.hpp>

TEST_CASE("Script Engine - Variable Manipulation", "[ScriptEngine]")
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

    SECTION("Test Getting/Setting Script Variables")
    {
        Vakol::LuaScript script = scriptEngine.CreateScript("testScripts/test.lua");
        Vakol::LuaType val = scriptEngine.GetScriptVariable(script, "abc");

        REQUIRE(val.as<int>() == 123);
        REQUIRE(script.env["abc"] == 123);

        scriptEngine.SetScriptVariable(script, "ABC", 1);
        REQUIRE(script.env["ABC"] == 1);
    }
}

TEST_CASE("Script Engine - Script Creation", "[ScriptEngine]")
{
    Vakol::ScriptEngine scriptEngine;

    SECTION("Test Creating Scripts")
    {
        Vakol::LuaScript script = scriptEngine.CreateScript("testScripts/test.lua");
        REQUIRE(script.path == "testScripts/test.lua");

        scriptEngine.SetGlobalVariable("test", 1);
        REQUIRE(script.env["test"] == 1);
    }
}

TEST_CASE("Script Engine - Script Function Tests", "[ScriptEngine]")
{
    Vakol::ScriptEngine scriptEngine;

    SECTION("Test Init Function")
    {
        Vakol::LuaScript script = scriptEngine.CreateScript("testScripts/test.lua");

        scriptEngine.InitScript(script);

        REQUIRE(script.env["initVal"] == 69);
    }

    SECTION("Test Update Function")
    {
        Vakol::LuaScript script = scriptEngine.CreateScript("testScripts/test.lua");

        scriptEngine.UpdateScript(script);

        REQUIRE(script.env["updateVal"] == 420);
    }

    SECTION("Test Tick Function")
    {
        Vakol::LuaScript script = scriptEngine.CreateScript("testScripts/test.lua");

        scriptEngine.TickScript(script);

        REQUIRE(script.env["tickVal"] == 1337);
    }

    SECTION("Test Phys Update Function")
    {
        Vakol::LuaScript script = scriptEngine.CreateScript("testScripts/test.lua");

        scriptEngine.PhysUpdateScript(script);

        REQUIRE(script.env["physUpdateVal"] == 69420);
    }
}

TEST_CASE("Script Engine - Function Definitions", "[ScriptEngine]")
{
    Vakol::ScriptEngine scriptEngine;

    SECTION("Test Set Global Function")
    {

        scriptEngine.SetGlobalFunction("global_add", [](int a, int b) -> int { return a + b; });

        Vakol::LuaScript script = scriptEngine.CreateScript("testScripts/global.lua");
        scriptEngine.InitScript(script); // used to set the result variable

        int result = scriptEngine.GetScriptVariable(script, "result").as<int>();
        REQUIRE(result == 7);
    }

    SECTION("Test Set Script Function")
    {
        Vakol::LuaScript script = scriptEngine.CreateScript("testScripts/test.lua");
        scriptEngine.SetScriptFunction(script, "script_multiply", [](int a, int b) -> int { return a * b; });

        // Access the function via the script's environment
        int result = script.env["script_multiply"](3, 4);
        REQUIRE(result == 12);

        // Ensure the function isn't globally available
        Vakol::LuaScript script2 = scriptEngine.CreateScript("testScripts/global.lua");

        bool hasGlobalFunction = script2.env["script_multiply"].valid();

        REQUIRE(hasGlobalFunction == false);
    }
}