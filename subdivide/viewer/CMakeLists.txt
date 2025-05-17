# Find required packages
find_package(OpenGL REQUIRED)

# Find GLFW using find_package first, then fall back to pkg-config
find_package(glfw3 QUIET)
if(NOT glfw3_FOUND)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW3 REQUIRED glfw3)
    set(GLFW3_LIBRARIES ${GLFW3_LIBRARIES} dl)
    include_directories(${GLFW3_INCLUDE_DIRS})
    link_directories(${GLFW3_LIBRARY_DIRS})
else()
    set(GLFW3_LIBRARIES glfw)
endif()

# Find source files
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
list(SORT SOURCES)  # For deterministic builds

# Create the viewer library
add_library(viewer ${SOURCES})

# Ensure C++11 is used
target_compile_features(viewer PRIVATE cxx_std_11)

# Include directories
target_include_directories(viewer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../qvlib/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link with dependencies
target_link_libraries(viewer PRIVATE
    subdivide
    qvlib
    ${OPENGL_LIBRARIES}
    ${GLFW3_LIBRARIES}
    dl
)

# Add X11 libraries for Linux
if(UNIX AND NOT APPLE)
    target_link_libraries(viewer PRIVATE X11)
endif()
