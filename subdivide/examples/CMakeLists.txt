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

target_link_libraries(glfw_subviewer
    PRIVATE
        viewer
        ${OPENGL_LIBRARIES}
        ${GLFW3_LIBRARIES}
)

target_include_directories(glfw_subviewer SYSTEM PRIVATE ${GLFW3_INCLUDE_DIRS})

# Install targets
install(TARGETS ccsub loopsub glfw_subviewer
    RUNTIME DESTINATION bin
)
