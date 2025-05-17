# Find all source files
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
list(SORT SOURCES)  # For deterministic builds

# Create the library
add_library(qvlib ${SOURCES})

# Ensure C++11 is used
target_compile_features(qvlib PRIVATE cxx_std_11)

# Include directories
target_include_directories(qvlib
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
