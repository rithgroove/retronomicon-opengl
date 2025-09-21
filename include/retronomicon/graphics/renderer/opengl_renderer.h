#pragma once

#include "retronomicon/graphics/renderer/i_renderer.h"
#include <SDL.h>
#include <string>

// include glad for GL function loading
#include <glad/gl.h>

namespace retronomicon::opengl {

    /**
     * @brief OpenGL-based implementation of the IRenderer interface.
     */
    class OpenGLRenderer : public retronomicon::graphics::renderer::IRenderer {
    public:
        /**
         * @brief Construct an OpenGLRenderer with given window title, width, and height.
         * @param title The window title.
         * @param width Window width in pixels.
         * @param height Window height in pixels.
         */
        OpenGLRenderer(const std::string& title, int width, int height);

        /**
         * @brief Destructor shuts down OpenGL/SDL if not already done.
         */
        virtual ~OpenGLRenderer();

        /// Initialize SDL, create window, and load GL functions.
        void init() override;

        /// Render a test background color.
        void render() override;

        /// Clean up OpenGL/SDL resources.
        void shutdown() override;

    private:
        std::string m_title;
        int m_width;
        int m_height;

        SDL_Window* m_window = nullptr;
        SDL_GLContext m_context = nullptr;
        bool m_initialized = false;
    };

} // namespace retronomicon::opengl
