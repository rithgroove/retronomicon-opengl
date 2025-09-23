#include "retronomicon/graphics/opengl_window.h"

namespace retronomicon::opengl::graphics {

    OpenGLWindow::OpenGLWindow(const std::string& title, int width, int height)
        : IWindow(title, width, height)
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            throw std::runtime_error("Failed to initialize SDL: " + std::string(SDL_GetError()));
        }

        // Set OpenGL attributes BEFORE creating window
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        m_window = SDL_CreateWindow(
            m_title.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            m_width,
            m_height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
        );

        if (!m_window) {
            throw std::runtime_error("Failed to create SDL window: " + std::string(SDL_GetError()));
        }

        m_glContext = SDL_GL_CreateContext(m_window);
        if (!m_glContext) {
            throw std::runtime_error("Failed to create OpenGL context: " + std::string(SDL_GetError()));
        }

        // Enable VSync
        if (SDL_GL_SetSwapInterval(1) < 0) {
            throw std::runtime_error("Failed to enable VSync: " + std::string(SDL_GetError()));
        }
    }

    OpenGLWindow::~OpenGLWindow() {
        if (m_glContext) {
            SDL_GL_DeleteContext(m_glContext);
        }
        if (m_window) {
            SDL_DestroyWindow(m_window);
        }
        SDL_Quit();
    }

    bool OpenGLWindow::isFullscreen() const {
        Uint32 flags = SDL_GetWindowFlags(m_window);
        return (flags & SDL_WINDOW_FULLSCREEN) || (flags & SDL_WINDOW_FULLSCREEN_DESKTOP);
    }

    void OpenGLWindow::toggleFullscreen() {
        if (isFullscreen()) {
            SDL_SetWindowFullscreen(m_window, 0); // windowed
        } else {
            SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        }
    }

    void OpenGLWindow::handleResize(int newWidth, int newHeight) {
        IWindow::handleResize(newWidth, newHeight);
        SDL_SetWindowSize(m_window, newWidth, newHeight);
        // For OpenGL: resize viewport
        glViewport(0, 0, newWidth, newHeight);
    }

    void OpenGLWindow::swapBuffers() {
        SDL_GL_SwapWindow(m_window);
    }

} // namespace retronomicon::opengl:
