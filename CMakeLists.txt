cmake_minimum_required(VERSION 3.15)
project(retronomicon-opengl VERSION 0.1 LANGUAGES C CXX)

# C++ settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output folders
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(RETRO_OPENGL_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "Retronomicon Opengl include path")
set(RETRO_OPENGL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include CACHE PATH "Retronomicon Opengl include path")

# External libraries
add_subdirectory(external/SDL2)
add_subdirectory(external/glad)

# Engine source (will add retronomicon-opengl library)
add_subdirectory(src)

target_link_libraries(retronomicon-opengl
    PUBLIC
        glad
        SDL2  
)

# Public include paths
target_include_directories(retronomicon-opengl
    PUBLIC
        ${RETRO_DIR}/include
        ${RETRO_OPENGL_DIR}/include
    PRIVATE
        ${RETRO_OPENGL_DIR}/external/glad/include
        ${RETRO_OPENGL_DIR}/external/SDL2/include
)

# Optional: Message info
message(STATUS "Building retronomicon-opengl as a library ✅")
