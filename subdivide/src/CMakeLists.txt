# Find all source files except triangleiter.cpp
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/triangleiter.cpp")
list(SORT SOURCES)  # For deterministic builds

# Create the library
add_library(subdivide ${SOURCES})

# Ensure C++11 is used
target_compile_features(subdivide PRIVATE cxx_std_11)


# Include directories
target_include_directories(subdivide
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
        ${CMAKE_SOURCE_DIR}/qvlib/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# Link with qvlib
target_link_libraries(subdivide PRIVATE qvlib)
