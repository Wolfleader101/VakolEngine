option(ASSIMP_BUILD_TESTS OFF)
option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(OPTION_BUILD_TESTS OFF)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_EXAMPLES OFF)

option(RP3D_COMPILE_TESTBED OFF)
option(RP3D_COMPILE_TESTS OFF)
option(RP3D_PROFILING_ENABLED OFF)
option(RP3D_CODE_COVERAGE_ENABLED OFF)
option(RP3D_DOUBLE_PRECISION_ENABLED OFF)

add_subdirectory(glad)
add_subdirectory(glfw)
add_subdirectory(spdlog)
add_subdirectory(imgui)
add_subdirectory(glm)
add_subdirectory(entt INTERFACE)
add_subdirectory(nlohmann_json)
add_subdirectory(reactphysics3d)
add_subdirectory(sol2)
add_subdirectory(lua)
add_subdirectory(assimp)



target_include_directories(imgui PUBLIC glfw/include)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
    glad/include
    glfw/include
	glm
    spdlog/include
    stb_image/
    glm
    entt/src
    nlohmann_json/include
    reactphysics3d/include
    sol2/include
    assimp/include
    lua/lua
)