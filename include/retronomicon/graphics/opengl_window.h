#pragma once

#include <stdexcept>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "retronomicon/graphics/i_window.h"

namespace retronomicon::opengl::graphics {

    /**
     * @class OpenGLWindow
     * @brief GLFW-based OpenGL window implementation.
     *
     * This class provides a concrete OpenGL window backend using GLFW.
     * It is responsible for:
     *  - Creating and destroying the native window
     *  - Managing the OpenGL context
     *  - Handling fullscreen toggling
     *  - Processing resize events
     *  - Polling window and input events
     *
     * The window owns the OpenGL context and is expected to live
     * for the duration of rendering.
     */
    class OpenGLWindow : public retronomicon::graphics::IWindow {
    public:
        /**
         * @brief Creates an OpenGL window.
         *
         * Initializes GLFW, creates a window, and sets up the
         * OpenGL context.
         *
         * @param title Window title.
         * @param width Initial window width in pixels.
         * @param height Initial window height in pixels.
         *
         * @throws std::runtime_error If window or context creation fails.
         */
        OpenGLWindow(const std::string& title, int width, int height);

        /**
         * @brief Destroys the window and releases associated resources.
         *
         * Terminates the OpenGL context and destroys the GLFW window.
         */
        ~OpenGLWindow() override;

        /**
         * @brief Checks whether the window is currently fullscreen.
         *
         * @return true if the window is in fullscreen mode.
         */
        bool isFullscreen() const override;

        /**
         * @brief Toggles fullscreen mode.
         *
         * Switches between windowed and fullscreen states while
         * preserving the previous window size.
         */
        void toggleFullscreen() override;

        /**
         * @brief Handles a window resize event.
         *
         * Updates internal dimensions and adjusts the OpenGL viewport.
         *
         * @param newWidth New window width in pixels.
         * @param newHeight New window height in pixels.
         */
        void handleResize(int newWidth, int newHeight) override;

        /**
         * @brief Swaps the front and back buffers.
         *
         * Should be called once per frame after rendering.
         */
        void swapBuffers();

        /**
         * @brief Polls and processes window events.
         *
         * This includes input, resize, and window close events.
         * Should be called once per frame.
         */
        void pollEvents();

        /**
         * @brief Gets the underlying GLFW window handle.
         *
         * Intended for low-level integrations or platform-specific hooks.
         *
         * @return Pointer to the GLFWwindow.
         */
        GLFWwindow* getGLFWwindow() const { return m_window; }

    private:
        /** Native GLFW window handle */
        GLFWwindow* m_window = nullptr;

        /** Current window width in pixels */
        int m_width = 0;

        /** Current window height in pixels */
        int m_height = 0;
    };

} // namespace retronomicon::opengl::graphics
