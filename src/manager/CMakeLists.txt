file(GLOB_RECURSE OPENGL_MANAGER_PLUGIN CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(retronomicon-opengl-manager OBJECT ${OPENGL_MANAGER_PLUGIN})

target_include_directories(
    retronomicon-opengl-manager PUBLIC
    ${RETRO_DIR}/include
    ${RETRO_OPENGL_DIR}/include
    ${RETRO_OPENGL_DIR}/external/glfw/include
    ${RETRO_OPENGL_DIR}/external/glad/include
)

target_link_libraries(retronomicon-opengl-manager
        retronomicon-manager
        glad           # static glad
        glfw
        )