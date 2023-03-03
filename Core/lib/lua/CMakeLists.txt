project(lua)

add_library(lua STATIC)

target_include_directories(lua SYSTEM PUBLIC src)

file(GLOB lua_LUA src/*.c)
target_sources(lua PRIVATE ${lua_LUA})

