# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# Find source files
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
list(SORT SOURCES)  # For deterministic builds

# Create the viewer library
add_library(viewer ${SOURCES})

# Ensure C++11 is used
target_compile_features(viewer PRIVATE cxx_std_11)

# Include directories
target_include_directories(viewer
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../qvlib/include
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link with dependencies
target_link_libraries(viewer
    PRIVATE 
        subdivide
        qvlib
    PUBLIC 
        OpenGL::GL
        glfw
        glm::glm
)

# Add X11 libraries for Linux
if(UNIX AND NOT APPLE)
    target_link_libraries(viewer PRIVATE X11)
endif()
