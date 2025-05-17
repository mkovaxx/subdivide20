cmake_minimum_required(VERSION 3.10)
project(Subdivide20 VERSION 2.0 LANGUAGES CXX)

# Set C++ standard globally
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable C++11 for all targets
set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE BOOL "Require C++11 standard")
set(CMAKE_CXX_EXTENSIONS OFF CACHE BOOL "Disable compiler-specific extensions")

# Default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Compiler flags
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
    
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g)
    else()
        add_compile_options(-O3)
    endif()
endif()

# Find GLFW using CMake's find_package
find_package(glfw3 3.3 REQUIRED)
if (NOT TARGET glfw)
    # If the imported target doesn't exist, create an interface library
    add_library(glfw INTERFACE)
    target_link_libraries(glfw INTERFACE ${GLFW3_LIBRARY})
    target_include_directories(glfw INTERFACE ${GLFW3_INCLUDE_DIRS})
    target_compile_definitions(glfw INTERFACE -DGLFW_INCLUDE_NONE)  # Don't include GLFW/gl.h
endif()

# Add subdirectories
add_subdirectory(qvlib)
add_subdirectory(subdivide)

# Create a convenience target that depends on all libraries
add_custom_target(all_libs)
add_dependencies(all_libs qvlib subdivide)
