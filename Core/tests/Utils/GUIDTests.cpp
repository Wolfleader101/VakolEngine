#include <catch2/catch_all.hpp>

#include <Utils/GUID.hpp>

TEST_CASE("GUID class tests", "[GUID]")
{
    Vakol::GUID guid1;
    Vakol::GUID guid2;

    SECTION("Test default constructor and IsValid function")
    {
        REQUIRE(guid1.IsValid() == true);
    }

    SECTION("Test GenNewGUID function")
    {
        auto old_guid_str = guid1.ConvertToString();
        guid1.GenNewGUID();
        auto new_guid_str = guid1.ConvertToString();
        REQUIRE(old_guid_str != new_guid_str);
    }

    SECTION("Test ConvertToString function")
    {
        auto guid_str = guid1.ConvertToString();
        REQUIRE(guid_str.length() == 36); // Typically a GUID is 36 characters long
    }

    SECTION("Test equality and inequality operators")
    {
        guid2 = guid1;
        REQUIRE(guid1 == guid2);
        REQUIRE_FALSE(guid1 != guid2);
    }

    SECTION("Test less than operator")
    {
        guid1.GenNewGUID();
        guid2.GenNewGUID();
        // This test might sometimes fail as GUID generation is random
        // and it's not guaranteed that guid1 will be less than guid2
        // So this test is to check the operation of the '<' operator, not to guarantee the order of the GUIDs
        REQUIRE(((guid1 < guid2) || (guid2 < guid1)));
    }

    // SECTION("Test cereal save and load")
    // {
    //     std::stringstream ss;
    //     {
    //         cereal::JSONOutputArchive oarchive(ss);
    //         guid1.save(oarchive);
    //     }
    //     {
    //         Vakol::GUID guid3;
    //         cereal::JSONInputArchive iarchive(ss);
    //         guid3.load(iarchive);
    //         REQUIRE(guid3.ConvertToString() == guid1.ConvertToString());
    //     }
    // }
}