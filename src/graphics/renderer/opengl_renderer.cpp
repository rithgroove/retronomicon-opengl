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
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        // Request OpenGL 3.3 Core profile
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
        if (!m_window) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(m_window);

        if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
            throw std::runtime_error("Failed to initialize GLAD");
        }

        glViewport(0, 0, m_width, m_height);
        glClearColor(0.1f, 0.1f, 0.3f, 1.0f);

        m_initialized = true;

        std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    }

    void OpenGLRenderer::render() {
        if (!m_initialized) return;

        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    void OpenGLRenderer::shutdown() {
        if (m_window) {
            glfwDestroyWindow(m_window);
            m_window = nullptr;
        }
        if (m_initialized) {
            glfwTerminate();
            m_initialized = false;
        }
    }

    bool OpenGLRenderer::shouldClose() const {
        return m_window ? glfwWindowShouldClose(m_window) : true;
    }

} // namespace retronomicon::opengl
