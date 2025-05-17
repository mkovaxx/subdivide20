set(SOURCES
    viewer.cpp
)

add_executable(subviewer ${SOURCES})

# Include directories
target_include_directories(subviewer
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../qvlib/include
)

# Link with dependencies
target_link_libraries(subviewer
    subdivide
    qvlib
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
)

# Add X11 libraries for Linux
if(UNIX AND NOT APPLE)
    target_link_libraries(subviewer X11)
endif()
