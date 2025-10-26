#pragma once

#include "retronomicon/graphics/renderer/i_renderer.h"
#include <string>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace retronomicon::opengl::graphics::renderer {
    using retronomicon::graphics::Texture;
    using retronomicon::math::Vec2;
    /**
     * @brief OpenGL-based implementation of the IRenderer interface using GLFW.
     */
    class OpenGLRenderer : public retronomicon::graphics::renderer::IRenderer {
        public:
            OpenGLRenderer(const std::string& title, int width, int height);
            ~OpenGLRenderer();

            void init();
            void clear();
            void render(std::shared_ptr<Texture> texture,
                        const Vec2& position,
                        const Vec2& scale,
                        float rotation = 0.0f,
                        float alpha = 1.0f);
            void show();
            void shutdown();

            bool shouldClose() const;

        private:
            unsigned int compileShader(unsigned int type, const char* src);
            unsigned int createShaderProgram(const char* vertSrc, const char* fragSrc);

            std::string m_title;
            int m_width;
            int m_height;

            GLFWwindow* m_window = nullptr;
            bool m_initialized = false;

            unsigned int m_VAO = 0;
            unsigned int m_VBO = 0;
            unsigned int m_shaderProgram = 0;
    };


} // namespace retronomicon::opengl
