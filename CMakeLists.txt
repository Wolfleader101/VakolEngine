# CMakeList.txt : CMake project for Vakol_Engine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("Vakol_Engine")

# Add source to this project's executable.
add_executable (Vakol_Engine "Vakol_Engine.cpp" "Vakol_Engine.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Vakol_Engine PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
