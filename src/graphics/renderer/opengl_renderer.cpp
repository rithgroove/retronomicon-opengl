#include "retronomicon/graphics/renderer/opengl_renderer.h"
#include <iostream>

namespace retronomicon::opengl {

    OpenGLRenderer::OpenGLRenderer(int width, int height, const char* title)
        : screenWidth(width), screenHeight(height), windowTitle(title) {}

    void OpenGLRenderer::init() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            throw std::runtime_error("Failed to init SDL: " + std::string(SDL_GetError()));
        }

        // Ask SDL for OpenGL 3.3 Core
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        window = SDL_CreateWindow(
            windowTitle,
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            screenWidth, screenHeight,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
        );

        if (!window) {
            throw std::runtime_error("Failed to create SDL Window: " + std::string(SDL_GetError()));
        }

        glContext = SDL_GL_CreateContext(window);
        if (!glContext) {
            throw std::runtime_error("Failed to create OpenGL context: " + std::string(SDL_GetError()));
        }

        if (glewInit() != GLEW_OK) {
            throw std::runtime_error("Failed to init GLEW");
        }

        glViewport(0, 0, screenWidth, screenHeight);
        glClearColor(0.1f, 0.1f, 0.3f, 1.0f);

        std::cout << "OpenGL initialized! Version: " << glGetString(GL_VERSION) << std::endl;
    }

    void OpenGLRenderer::render() {
        glClear(GL_COLOR_BUFFER_BIT);
        // TODO: draw calls here later
        SDL_GL_SwapWindow(window);
    }

    void OpenGLRenderer::shutdown() {
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    OpenGLRenderer::~OpenGLRenderer() {
        shutdown();
    }

}
