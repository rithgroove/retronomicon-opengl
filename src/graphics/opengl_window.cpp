#include "retronomicon/graphics/opengl_window.h"
#include <stdexcept>
#include <iostream>

namespace retronomicon::opengl::graphics {

OpenGLWindow::OpenGLWindow(const std::string& title, int width, int height)
    : IWindow(title, width, height), m_width(width), m_height(height)
{
    // --------------------------------------------------
    // GLFW Init (only once)
    // --------------------------------------------------
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    // Configure GLFW for a modern OpenGL context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // --------------------------------------------------
    // Create the window + context
    // --------------------------------------------------
    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!m_window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(m_window);

    // --------------------------------------------------
    // Load OpenGL functions using GLAD (only once)
    // --------------------------------------------------
    if (!gladLoadGL(glfwGetProcAddress)) {
        glfwDestroyWindow(m_window);
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }

    std::cout << "OpenGL initialized: " << glGetString(GL_VERSION) << std::endl;
}

OpenGLWindow::~OpenGLWindow() {
    if (m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }

    glfwTerminate();
}

bool OpenGLWindow::isFullscreen() const {
    return glfwGetWindowMonitor(m_window) != nullptr;
}

void OpenGLWindow::toggleFullscreen() {
    if (!m_window) return;

    if (isFullscreen()) {
        // Return to windowed mode
        glfwSetWindowMonitor(m_window, nullptr, 100, 100, m_width, m_height, 0);
    } else {
        GLFWmonitor* mon = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(mon);

        glfwSetWindowMonitor(m_window,
                             mon,
                             0, 0,
                             mode->width,
                             mode->height,
                             mode->refreshRate);
    }
}

void OpenGLWindow::handleResize(int newWidth, int newHeight) {
    m_width  = newWidth;
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
