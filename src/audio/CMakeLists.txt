file(GLOB_RECURSE OPENGL_AUDIO_PLUGIN CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(retronomicon-opengl-audio OBJECT ${OPENGL_AUDIO_PLUGIN})

target_include_directories(
    retronomicon-opengl-audio PUBLIC
    ${RETRO_DIR}/include
    ${RETRO_OPENGL_DIR}/include
    ${RETRO_OPENGL_DIR}/external/openal/include
)

target_link_libraries(retronomicon-opengl-audio
        glfw           # static glfw
        OpenAL
        )