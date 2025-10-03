#pragma once

#include "retronomicon/graphics/renderer/i_renderer.h"
#include <string>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace retronomicon::opengl {

    /**
     * @brief OpenGL-based implementation of the IRenderer interface using GLFW.
     */
    class OpenGLRenderer : public retronomicon::graphics::renderer::IRenderer {
    public:
        OpenGLRenderer(const std::string& title, int width, int height);
        virtual ~OpenGLRenderer();

        /// Initialize GLFW, create window, and load GL functions.
        void init() override;

        /// Render a test background color.
        void render() override;

        /// Clean up OpenGL/GLFW resources.
        void shutdown() override;

        /// Check if window should close.
        bool shouldClose() const;

    private:
        std::string m_title;
        int m_width;
        int m_height;

        GLFWwindow* m_window = nullptr;
        bool m_initialized = false;
    };

} // namespace retronomicon::opengl
