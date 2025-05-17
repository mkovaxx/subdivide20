set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Catmull-Clark subdivision tool
add_executable(ccsub ccsub.cpp)

target_include_directories(ccsub
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
        ${CMAKE_CURRENT_SOURCE_DIR}/../../qvlib/include
)

target_link_libraries(ccsub
    PRIVATE
        subdivide
        qvlib
)

# Loop subdivision tool
add_executable(loopsub loopsub.cpp)

target_include_directories(loopsub
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
        ${CMAKE_CURRENT_SOURCE_DIR}/../../qvlib/include
)

target_link_libraries(loopsub
    PRIVATE
        subdivide
        qvlib
)

# GLFW-based subviewer
add_executable(glfw_subviewer glfw_subviewer.cpp)

target_include_directories(glfw_subviewer
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
        ${CMAKE_CURRENT_SOURCE_DIR}/../../qvlib/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../viewer
)

# Link against GLFW and OpenGL
target_link_libraries(glfw_subviewer
    PRIVATE
        viewer
        glfw
        OpenGL::GL
)

# Add OpenGL include directories
find_package(OpenGL REQUIRED)
target_include_directories(glfw_subviewer PRIVATE ${OPENGL_INCLUDE_DIR})

# Install targets
install(TARGETS ccsub loopsub glfw_subviewer
    RUNTIME DESTINATION bin
)
