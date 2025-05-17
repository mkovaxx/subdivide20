# Set the C++ standard and compiler warnings
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use file globbing to find all .cpp files in the current directory
file(GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

# Remove triangleiter.cpp if it was included
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/triangleiter.cpp")

# Sort the sources for deterministic builds
list(SORT SOURCES)

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

# Set version properties
set_target_properties(subdivide PROPERTIES
    VERSION 1.0.0
    SOVERSION 1
)
