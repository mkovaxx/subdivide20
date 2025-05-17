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

# Install targets
install(TARGETS ccsub loopsub
    RUNTIME DESTINATION bin
)
