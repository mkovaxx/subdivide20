# Add pedantic flag for subdivide components
add_compile_options(-pedantic)

# Include directories for all subdirectories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/template
    ${CMAKE_SOURCE_DIR}/qvlib/include
)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(viewer)
add_subdirectory(examples)
