cmake_minimum_required (VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source to this project's executable.
add_executable (${PROJECT_NAME} "Demo.cpp")

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
    VakolCore/src
)

target_link_libraries(${PROJECT_NAME} PUBLIC
VakolCore
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/Demo/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets)