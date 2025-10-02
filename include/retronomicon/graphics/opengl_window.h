#pragma once

#include <stdexcept>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "retronomicon/graphics/i_window.h"

namespace retronomicon::opengl::graphics {

/**
 * @brief OpenGL-capable window implementation of IWindow.
 *
 * Uses GLFW to create an OpenGL context and manage the window.
 */
class OpenGLWindow : public retronomicon::graphics::IWindow {
    public:
        /**
         * @brief Construct a new OpenGLWindow object.
         *
         * @param title   Title of the window.
         * @param width   Initial width in pixels.
         * @param height  Initial height in pixels.
         *
         * @throws std::runtime_error if SDL or OpenGL context creation fails.
         */
        OpenGLWindow(const std::string& title, int width, int height);

        /**
         * @brief Destroy the OpenGLWindow object.
         *
         * Cleans up SDL and OpenGL resources.
         */
        ~OpenGLWindow() override;

        // ------------------------------------------------------------------------
        // IWindow overrides
        // ------------------------------------------------------------------------

        bool isFullscreen() const override;
        void toggleFullscreen() override;
        void handleResize(int newWidth, int newHeight) override;

        // ------------------------------------------------------------------------
        // Extra utilities
        // ------------------------------------------------------------------------

        /**
         * @brief Swap the OpenGL buffers.
         */
        void swapBuffers();

        /**
         * @brief Get raw GLFWwindow pointer.
         */
        GLFWwindow* getGLFWwindow() const { return m_window; }

    private:
        GLFWwindow* m_window = nullptr; ///< GLFW window handle.
        int m_width;
        int m_height;
    };

} // namespace retronomicon::opengl::graphics
