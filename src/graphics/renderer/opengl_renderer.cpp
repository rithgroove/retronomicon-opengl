#include "retronomicon/graphics/renderer/opengl_renderer.h"
#include <stdexcept>
#include <iostream>

namespace retronomicon::opengl {

    OpenGLRenderer::OpenGLRenderer(const std::string& title, int width, int height)
        : m_title(title), m_width(width), m_height(height) {}

    OpenGLRenderer::~OpenGLRenderer() {
        shutdown();
    }

    void OpenGLRenderer::init() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            throw std::runtime_error("Failed to initialize SDL: " + std::string(SDL_GetError()));
        }

        // Request a Core GL 3.0+ context (change to ES profile if building GLES)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        m_window = SDL_CreateWindow(
            m_title.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            m_width,
            m_height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
        );

        if (!m_window) {
            throw std::runtime_error("Failed to create SDL window: " + std::string(SDL_GetError()));
        }

        m_context = SDL_GL_CreateContext(m_window);
        if (!m_context) {
            throw std::runtime_error("Failed to create OpenGL context: " + std::string(SDL_GetError()));
        }

        // Load GL functions via GLAD2
        if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress)) {
            throw std::runtime_error("Failed to initialize GLAD");
        }

        glViewport(0, 0, m_width, m_height);
        glClearColor(0.1f, 0.1f, 0.3f, 1.0f);

        m_initialized = true;
        int major = 0, minor = 0;
        SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
        SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor);

        std::cout << "OpenGL version: " << major << "." << minor << std::endl;
    }

    void OpenGLRenderer::render() {
        if (!m_initialized) return;

        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(m_window);
    }

    void OpenGLRenderer::shutdown() {
        if (m_context) {
            SDL_GL_DeleteContext(m_context);
            m_context = nullptr;
        }
        if (m_window) {
            SDL_DestroyWindow(m_window);
            m_window = nullptr;
        }
        if (m_initialized) {
            SDL_Quit();
            m_initialized = false;
        }
    }

} // namespace retronomicon::opengl
