file(GLOB_RECURSE OPENGL_GRAPHIC_PLUGIN CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(retronomicon-opengl-graphics OBJECT ${OPENGL_GRAPHIC_PLUGIN})

target_include_directories(
    retronomicon-opengl-graphics PUBLIC
    ${RETRO_DIR}/include
    ${RETRO_OPENGL_DIR}/include
    ${RETRO_OPENGL_DIR}/external/SDL2/include
    ${RETRO_OPENGL_DIR}/external/glfw/include
    ${RETRO_OPENGL_DIR}/external/glad/include
)

target_link_libraries(retronomicon-opengl-graphics
        glad           # static glad
        SDL2::SDL2     # static SDL2 (change to SDL2::SDL2-static if needed)

        )