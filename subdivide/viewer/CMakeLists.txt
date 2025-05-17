# Find OpenGL and GLUT
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

set(SOURCES
    viewer.cpp
    camera.cpp
    arcball.cpp
    pickviewer.cpp
    ballviewer.cpp
    geoobject.cpp
    pickobject.cpp
    pickabletri.cpp
    pickablequad.cpp
    pickableparam.cpp
)

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
