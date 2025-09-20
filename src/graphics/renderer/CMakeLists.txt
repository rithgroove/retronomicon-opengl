file(GLOB_RECURSE OPENGL_GRAPHIC_PLUGIN CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(retronomicon-opengl-graphics OBJECT ${OPENGL_GRAPHIC_PLUGIN})

target_include_directories(
    retronomicon-opengl-graphics PUBLIC
    ${RETRO_DIR}/include
    ${RETRO_SDL_DIR}/include
    ${RETRO_SDL_DIR}/external/SDL2/include
)
