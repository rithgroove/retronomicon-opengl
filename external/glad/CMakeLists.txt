cmake_minimum_required(VERSION 3.10)

# Create glad library (both GL + GLES2)
add_library(glad STATIC
    src/gl.c
    src/gles2.c
)
option(USE_GLES "Build with OpenGL ES 2.x instead of desktop OpenGL" OFF)

# Public include directory (so consumers can #include <glad/...>)
target_include_directories(glad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Optional: if you want to select between GL and GLES in code

#if (USE_GLES)
#    target_compile_definitions(glad PUBLIC GLAD_GLES2)
#else()
#    target_compile_definitions(glad PUBLIC GLAD_GL)
#endif()