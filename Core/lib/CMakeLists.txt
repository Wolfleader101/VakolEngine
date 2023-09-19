set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
set(BUILD_SHARED_LIBS OFF)
set(SET_BUILD_TESTS OFF)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF)
set(ASSIMP_INSTALL OFF)
set(ASSIMP_BUILD_ASSIMP_VIEW OFF)
set(ASSIMP_BUILD_OBJ_IMPORTER ON)
set(ASSIMP_BUILD_GLTF_IMPORTER ON)
set(ASSIMP_BUILD_FBX_IMPORTER ON)

set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_EXAMPLES OFF)

set(RP3D_COMPILE_TESTBED OFF)
set(RP3D_COMPILE_TESTS OFF)
set(RP3D_PROFILING_ENABLED OFF)
set(RP3D_CODE_COVERAGE_ENABLED OFF)
set(RP3D_DOUBLE_PRECISION_ENABLED OFF)

set(BUILD_DOC OFF)
set(BUILD_SANDBOX OFF)
set(SKIP_PERFORMANCE_COMPARISON ON)

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
add_subdirectory(crossguid)

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
    crossguid/include
)