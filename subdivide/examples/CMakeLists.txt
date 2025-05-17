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

# Subviewer example
add_executable(subviewer subviewer.cpp)

target_include_directories(subviewer
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
        ${CMAKE_CURRENT_SOURCE_DIR}/../../qvlib/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../viewer
)

target_link_libraries(subviewer
    PRIVATE
        viewer
        ${OPENGL_LIBRARIES}
        ${GLUT_LIBRARIES}
)

# Install targets
install(TARGETS ccsub loopsub subviewer
    RUNTIME DESTINATION bin
)
