# Find OpenGL and GLUT
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Use file globbing to find all .cpp files in the current directory
file(GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

# Sort the sources for deterministic builds
list(SORT SOURCES)

add_library(viewer ${SOURCES})

# Include directories
target_include_directories(viewer
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../qvlib/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link with dependencies
target_link_libraries(viewer
    subdivide
    qvlib
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
)

# Add X11 libraries for Linux
if(UNIX AND NOT APPLE)
    target_link_libraries(viewer X11)
endif()
