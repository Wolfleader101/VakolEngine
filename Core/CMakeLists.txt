cmake_minimum_required(VERSION 3.20)

SET(PROJECT_NAME VakolCore)
project(${PROJECT_NAME} LANGUAGES CXX)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)

# Add source to this project's executable.
add_library(${PROJECT_NAME} STATIC ${SOURCES})

add_subdirectory(lib)

target_include_directories(${PROJECT_NAME} PRIVATE ${GLOBAL_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/ ${GLOBAL_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PUBLIC
    imgui
    glad
    glfw
    spdlog
    glm
    reactphysics3d
    assimp
    lua
    sol2
    cereal
    crossguid
)

# Set compile flags.
target_compile_options(${PROJECT_NAME} PRIVATE

    # Clang
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:
    -Weverything -fcolor-diagnostics

    # Disable specific warnings.
    -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded
    -Wno-deprecated-declarations -Wno-exit-time-destructors
    -Wno-switch-enum -Wno-weak-vtables -Wno-global-constructors
    -Wno-newline-eof>

    # GCC
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic -fdiagnostics-color=always>

    # Visual Studio
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)