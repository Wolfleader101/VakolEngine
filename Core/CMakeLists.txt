cmake_minimum_required(VERSION 3.20)

SET(PROJECT_NAME VakolCore)
project(${PROJECT_NAME} LANGUAGES CXX)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source and test directories
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

SET(CopyResources FALSE)

function(setup_mylib_resources target_name)
    if(NOT CopyResources)
        add_custom_command(TARGET ${target_name} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${target_name}>/coreAssets)
    else()
        add_custom_command(TARGET ${target_name} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${target_name}>/coreAssets)
    endif()
endfunction()

########### TESTING ###########


add_subdirectory(tests)

# Create a test executable
add_executable(VakolTests tests/tests.cpp)

# Add test sources to the test executable
target_sources(VakolTests PRIVATE ${TESTS_SRC})

# Link Catch2 and your engine to the test executable
target_link_libraries(VakolTests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(VakolTests PRIVATE ${PROJECT_NAME})

# Set compile flags.
target_compile_options(VakolTests PRIVATE

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

setup_mylib_resources(VakolTests)

if(NOT CopyResources)
    add_custom_command(TARGET VakolTests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/scripts $<TARGET_FILE_DIR:VakolTests>/testScripts)
else()
    add_custom_command(TARGET VakolTests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/scripts $<TARGET_FILE_DIR:VakolTests>/testScripts)

endif()

if(NOT CopyResources)
    add_custom_command(TARGET VakolTests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/assets $<TARGET_FILE_DIR:VakolTests>/testAssets)
else()
    add_custom_command(TARGET VakolTests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/assets $<TARGET_FILE_DIR:VakolTests>/testAssets)
endif()