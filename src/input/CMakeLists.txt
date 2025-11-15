file(GLOB_RECURSE OPENGL_INPUT_PLUGIN CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(retronomicon-opengl-input OBJECT ${OPENGL_INPUT_PLUGIN})

target_include_directories(
    retronomicon-opengl-input PUBLIC
    ${RETRO_DIR}/include
    ${RETRO_OPENGL_DIR}/include
    ${RETRO_OPENGL_DIR}/external/SDL2/include
    ${RETRO_OPENGL_DIR}/external/glfw/include
    ${RETRO_OPENGL_DIR}/external/glad/include
    ${RETRO_OPENGL_DIR}/external/glm/include
)

target_link_libraries(retronomicon-opengl-input
        glad           # static glad
        glfw
        glm
        )