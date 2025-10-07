file(GLOB_RECURSE OPENGL_ASSET_PLUGIN CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(retronomicon-opengl-asset OBJECT ${OPENGL_ASSET_PLUGIN})

target_include_directories(
    retronomicon-opengl-asset PUBLIC
    ${RETRO_DIR}/include
    ${RETRO_OPENGL_DIR}/include
    ${RETRO_OPENGL_DIR}/external/glad/include
)

target_link_libraries(retronomicon-opengl-asset
        glfw           # static glfw
        )