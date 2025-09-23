#pragma once

#include <SDL.h>
#include <stdexcept>
#include <glad/gl.h>
#include "retronomicon/graphics/i_window.h"

namespace retronomicon::opengl::graphics {

/**
 * @brief OpenGL-capable window implementation of IWindow.
 *
 * Uses SDL2 to create an OpenGL context and manage the window.
 * This class is meant for rendering with OpenGL, unlike the SDLWindow
 * which uses SDL's 2D renderer.
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
         * @brief Get raw SDL_Window pointer.
         */
        SDL_Window* getSDLWindow() const { return m_window; }

        /**
         * @brief Get OpenGL context for this window.
         */
        SDL_GLContext getGLContext() const { return m_glContext; }

    private:
        SDL_Window* m_window = nullptr;   ///< SDL window handle.
        SDL_GLContext m_glContext = nullptr; ///< OpenGL context.
    };

} // namespace retronomicon::opengl::graphics
