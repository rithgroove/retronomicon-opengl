message(STATUS "==== Processing ${CMAKE_CURRENT_LIST_FILE} ====")

# Add submodules inside lib/
add_subdirectory(graphics)
add_subdirectory(manager)
#add_subdirectory(asset)
#add_subdirectory(audio)
add_subdirectory(input)

# Define the main retronomicon engine library
add_library(retronomicon-opengl)

# Link subsystem libraries
target_link_libraries(retronomicon-opengl
    PUBLIC
        retronomicon
        retronomicon-opengl-graphics
        retronomicon-opengl-manager
        #retronomicon-opengl-asset
        #retronomicon-opengl-audio
        retronomicon-opengl-input
)

