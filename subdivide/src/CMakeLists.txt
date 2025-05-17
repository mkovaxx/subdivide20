# Set the C++ standard and compiler warnings
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Source files for the subdivide library
set(SOURCES
    flatmesh.cpp
    ivgraph.cpp
    quadmanipulator.cpp
    quadmesh.cpp
    quadrule.cpp
    subquad.cpp
    subtri.cpp
    tagflatmesh.cpp
    tagivgraph.cpp
    triangle.cpp
    trianglering.cpp
    trimanipulator.cpp
    trimesh.cpp
    trirule.cpp
)

# Create the library
add_library(subdivide STATIC ${SOURCES})

# Set include directories
target_include_directories(subdivide
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        $<INSTALL_INTERFACE:include>
        ${CMAKE_SOURCE_DIR}/qvlib/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# Set compiler flags
target_compile_options(subdivide
    PRIVATE
        $<$<CONFIG:Debug>:-g>
)

# Link with qvlib
target_link_libraries(subdivide PRIVATE qvlib)

# Set output name to match the original Makefile
set_target_properties(subdivide PROPERTIES
    OUTPUT_NAME sub
    VERSION 1.0.0
    SOVERSION 1
)
