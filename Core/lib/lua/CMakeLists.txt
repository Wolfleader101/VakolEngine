project(lua)

add_library(lua STATIC)

target_include_directories(lua SYSTEM PUBLIC src)

file(GLOB LUA_SRC src/*.c)
target_sources(lua PRIVATE ${LUA_SRC})

