cmake_minimum_required (VERSION 3.8)

SET(PROJECT_NAME VakolEngine)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Core)
add_subdirectory(Demo)