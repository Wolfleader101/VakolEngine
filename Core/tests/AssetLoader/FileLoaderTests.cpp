#include <AssetLoader/FileLoader.hpp>
#include <catch2/catch_all.hpp>

TEST_CASE("Testing Loading Files", "[FileLoader]")
{
    SECTION("Loading Existing Files")
    {
        const std::string output = LoadFile("testAssets/testFile.txt");
        REQUIRE(output == "Hello World!");
    }

    SECTION("Loading Non-Existant Files")
    {
        const std::string output = LoadFile("testAssets/invalid.txt");
        REQUIRE(output.empty());
    }
}

TEST_CASE("Testing The Existence of Files", "[FileLoader]")
{
    SECTION("IsExists Existing Files")
    {
        const bool output = FileExists("testAssets/testFile.txt");
        REQUIRE(output);
    }

    SECTION("IsExists Non-Existant Files")
    {
        const bool output = FileExists("testAssets/invalid.txt");
        REQUIRE_FALSE(output);
    }
}

TEST_CASE("Testing The File Suffix of a File", "[FileLoader]")
{
    SECTION("Getting the File Suffix of an Existing File")
    {
        const std::string suffix = GetFileSuffix("testAssets/testFile.txt", '/');
        REQUIRE(suffix == "testFile.txt");
    }

    SECTION("Getting the File Suffix of a Non-Existant File")
    {
        const std::string suffix = GetFileSuffix("testAssets/invalid.txt", '.');
        REQUIRE(suffix == "txt");
    }
}

TEST_CASE("Testing The File Extension of a File", "[FileLoader]")
{
    SECTION("Getting the File Extension of an Existing File")
    {
        const std::string extension = GetFileExtension("testAssets/testFile.txt");
        REQUIRE(extension == ".txt");
    }

    SECTION("Getting the File Extension of a Non-Existant File")
    {
        const std::string extension = GetFileExtension("testAssets/invalid.txt");
        REQUIRE(extension == ".txt");
    }

    SECTION("Edge Case of a File Extension")
    {
        const std::string extension = GetFileExtension("testAs.sets/t.es.tFile.txt");
        REQUIRE(extension == ".txt");
    }
}