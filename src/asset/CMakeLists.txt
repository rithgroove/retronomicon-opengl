add_library(retronomicon-opengl-asset OBJECT
    ${CMAKE_CURRENT_SOURCE_DIR}/opengl_font_asset.cpp
)

target_include_directories(
    retronomicon-opengl-asset PUBLIC
    ${RETRO_DIR}/include
    ${RETRO_DIR}/external/stb
    ${RETRO_OPENGL_DIR}/include
    ${RETRO_OPENGL_DIR}/external/glad/include
    ${RETRO_OPENGL_DIR}/external/glfw/include
    ${RETRO_OPENGL_DIR}/external/openal/include
)

target_link_libraries(retronomicon-opengl-asset
        retronomicon
        glfw           # static glfw
        )
