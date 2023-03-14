SET(ASSIMP_BUILD_TESTS OFF)
SET(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
SET(SET_BUILD_TESTS OFF)

SET(GLFW_BUILD_DOCS OFF)
SET(GLFW_BUILD_TESTS OFF)
SET(GLFW_BUILD_EXAMPLES OFF)

SET(RP3D_COMPILE_TESTBED OFF)
SET(RP3D_COMPILE_TESTS OFF)
SET(RP3D_PROFILING_ENABLED OFF)
SET(RP3D_CODE_COVERAGE_ENABLED OFF)
SET(RP3D_DOUBLE_PRECISION_ENABLED OFF)


SET(BUILD_DOC OFF)
SET(BUILD_SANDBOX OFF)
SET(SKIP_PERFORMANCE_COMPARISON ON)

add_subdirectory(glad)
add_subdirectory(glfw)
add_subdirectory(spdlog)
add_subdirectory(imgui)
add_subdirectory(glm)
add_subdirectory(entt INTERFACE)
add_subdirectory(reactphysics3d)
add_subdirectory(sol2)
add_subdirectory(lua)
add_subdirectory(assimp)
add_subdirectory(cereal)

target_include_directories(imgui PUBLIC glfw/include)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
    glad/include
    glfw/include
	glm
    spdlog/include
    stb_image/
    glm
    entt/src
    reactphysics3d/include
    sol2/include
    assimp/include
    lua/lua
    cereal/include
)