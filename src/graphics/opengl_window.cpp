#include "retronomicon/graphics/opengl_window.h"
#include <stdexcept>

namespace retronomicon::opengl::graphics {

    OpenGLWindow::OpenGLWindow(const std::string& title, int width, int height)
        : IWindow(title,width,height)
    {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        // Configure GLFW for OpenGL context
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // e.g. OpenGL 3.3
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!m_window) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(m_window);

        // Load OpenGL functions via glad
        if (!gladLoadGL(glfwGetProcAddress)) {
            glfwDestroyWindow(m_window);
            glfwTerminate();
            throw std::runtime_error("Failed to initialize glad");
        }
    }

    OpenGLWindow::~OpenGLWindow() {
        if (m_window) {
            glfwDestroyWindow(m_window);
        }
        glfwTerminate();
    }

    bool OpenGLWindow::isFullscreen() const {
        return glfwGetWindowMonitor(m_window) != nullptr;
    }

    void OpenGLWindow::toggleFullscreen() {
        if (isFullscreen()) {
            // Switch back to windowed
            glfwSetWindowMonitor(m_window, nullptr, 100, 100, m_width, m_height, 0);
        } else {
            // Switch to fullscreen (primary monitor)
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        }
    }

    void OpenGLWindow::handleResize(int newWidth, int newHeight) {
        m_width = newWidth;
        m_height = newHeight;
        glViewport(0, 0, m_width, m_height);
    }

    void OpenGLWindow::swapBuffers() {
        glfwSwapBuffers(m_window);
    }

    void OpenGLWindow::pollEvents() {
        glfwPollEvents();
    }

} // namespace retronomicon::opengl::graphics
