# Find OpenGL and GLUT
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Silence deprecation warnings on macOS
add_definitions(-DGL_SILENCE_DEPRECATION)

set(SOURCES
    viewer.cpp
    camera.cpp
    arcball.cpp
)

add_library(viewer ${SOURCES})

# Include directories
target_include_directories(viewer
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../qvlib/include
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
