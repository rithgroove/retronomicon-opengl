#include "retronomicon/input/glfw_raw_input.h"

namespace retronomicon::opengl::input {

    GLFWRawInput::GLFWRawInput(GLFWwindow* window)
        : m_window(window), m_mouseX(0.0), m_mouseY(0.0), m_mouseButtons(0) {
    }

    void GLFWRawInput::poll() {
        m_events.clear();
        glfwPollEvents();

        // Mouse state
        glfwGetCursorPos(m_window, &m_mouseX, &m_mouseY);

        m_mouseButtons = 0;
        if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            m_mouseButtons |= 1 << 0;
        if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
            m_mouseButtons |= 1 << 1;
        if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
            m_mouseButtons |= 1 << 2;

        // For simplicity: collect events as strings
        // (real engine might wrap in a custom Event struct)
        if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            m_events.push_back("ESCAPE_PRESSED");
        }
    }

    void GLFWRawInput::clear() {
        m_events.clear();
    }

    const std::vector<std::string>& GLFWRawInput::getEvents() const {
        return m_events;
    }

    bool GLFWRawInput::isKeyPressed(int keyCode) const {
        return glfwGetKey(m_window, keyCode) == GLFW_PRESS;
    }

    int GLFWRawInput::getMouseX() const {
        return static_cast<int>(m_mouseX);
    }

    int GLFWRawInput::getMouseY() const {
        return static_cast<int>(m_mouseY);
    }

    unsigned int GLFWRawInput::getMouseButtons() const {
        return m_mouseButtons;
    }

} // namespace retronomicon::input
