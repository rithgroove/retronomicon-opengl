#include "retronomicon/graphics/renderer/opengl_renderer.h"
#include <glad.h>
#include <iostream>
#include <stdexcept>

namespace retronomicon::opengl {

    struct OpenGLRenderer::Impl {
        SDL_Window* window = nullptr;
        SDL_GLContext glContext = nullptr;
        int width;
        int height;
        const char* title;
    };

    OpenGLRenderer::OpenGLRenderer(int width, int height, const char* title) {
        impl = new Impl{nullptr, nullptr, width, height, title};
    }

    void OpenGLRenderer::init() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            throw std::runtime_error("Failed to init SDL: " + std::string(SDL_GetError()));
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        impl->window = SDL_CreateWindow(
            impl->title,
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            impl->width, impl->height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
        );

        if (!impl->window) {
            throw std::runtime_error("Failed to create SDL Window: " + std::string(SDL_GetError()));
        }

        impl->glContext = SDL_GL_CreateContext(impl->window);
        if (!impl->glContext) {
            throw std::runtime_error("Failed to create OpenGL context: " + std::string(SDL_GetError()));
        }

        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
            throw std::runtime_error("Failed to initialize GLAD");
        }

        glViewport(0, 0, impl->width, impl->height);
        glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
    }

    void OpenGLRenderer::render() {
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(impl->window);
    }

    void OpenGLRenderer::shutdown() {
        if (impl->glContext) SDL_GL_DeleteContext(impl->glContext);
        if (impl->window) SDL_DestroyWindow(impl->window);
        SDL_Quit();
    }

    OpenGLRenderer::~OpenGLRenderer() {
        shutdown();
        delete impl;
    }

}
