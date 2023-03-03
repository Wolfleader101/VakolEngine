cmake_minimum_required (VERSION 3.8)

SET(PROJECT_NAME VakolCore)
project(${PROJECT_NAME})

find_package(OpenGL REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)

# Add source to this project's executable.
add_library (${PROJECT_NAME} STATIC ${SOURCES})
#add_library (${PROJECT_NAME} STATIC ${SOURCES} "lib/glad/src/glad.c")

#add_subdirectory(lib)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

#target_link_libraries(${PROJECT_NAME} PUBLIC
#imgui
#glad
#glfw
#spdlog
#glm
#nlohmann_json
#reactphysics3d
#)
