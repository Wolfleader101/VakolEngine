cmake_minimum_required (VERSION 3.20)

SET(PROJECT_NAME VakolEngine)
project(${PROJECT_NAME} LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(WarningsAsErrors "WarningsAsErrors" OFF)
option(ASAN_ENABLED "Build this target with AddressSanitizer" OFF)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG    ${PROJECT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG    ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE  ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG    ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE  ${PROJECT_BINARY_DIR}/lib)

set(CMAKE_DEBUG_POSTFIX "")
option(CopyFiles "CopyFiles" OFF)

if (MSVC)
    string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

if (MSVC)
  add_compile_options(/bigobj)
elseif (GNU)
  add_compile_options(-Wa,-mbig-obj)
else ()
  add_compile_options(-Bigobj)
endif ()

if (WarningsAsErrors)
    add_compile_options(${PROJECT_NAME} PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Werror>
        $<$<CXX_COMPILER_ID:MSVC>:/WX>
    )
endif()

if(ASAN_ENABLED)
  if(MSVC)
    add_compile_options(${PROJECT_NAME} PUBLIC /fsanitize=address, /permissive-)
  else()
    add_compile_options(${PROJECT_NAME} PUBLIC -fsanitize=address,undefined,leak)
    add_link_options(${PROJECT_NAME} PUBLIC -fsanitize=address,undefined,leak)
  endif()
endif()


add_subdirectory(Core)
add_subdirectory(Demo)

