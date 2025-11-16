#pragma once

#include <stdexcept>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "retronomicon/graphics/i_window.h"

namespace retronomicon::opengl::graphics {

class OpenGLWindow : public retronomicon::graphics::IWindow {
public:
    OpenGLWindow(const std::string& title, int width, int height);
    ~OpenGLWindow() override;

    bool isFullscreen() const override;
    void toggleFullscreen() override;
    void handleResize(int newWidth, int newHeight) override;

    void swapBuffers();
    void pollEvents();

    GLFWwindow* getGLFWwindow() const { return m_window; }

private:
    GLFWwindow* m_window = nullptr;
    int m_width = 0;
    int m_height = 0;
};

} // namespace retronomicon::opengl::graphics
