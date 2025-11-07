#pragma once

#include "retronomicon/graphics/renderer/i_renderer.h"
#include "retronomicon/graphics/opengl_color.h"

#include <string>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace retronomicon::opengl::graphics::renderer {
using retronomicon::graphics::Texture;
using retronomicon::math::Vec2;
using retronomicon::math::Rect;
using retronomicon::graphics::Color;
using retronomicon::opengl::graphics::OpenGLColor;

/**
 * @brief OpenGL-based implementation of the IRenderer interface using GLFW.
 */
class OpenGLRenderer : public retronomicon::graphics::renderer::IRenderer {
public:
    OpenGLRenderer(const std::string& title, int width, int height);
    ~OpenGLRenderer() override;

    void init() override;
    void clear() override;
    void render(std::shared_ptr<Texture> texture,
                const Vec2& position,
                const Vec2& scale,
                float rotation = 0.0f,
                float alpha = 1.0f) override;
    void renderQuad(std::shared_ptr<Texture> texture,
                    const Rect& target,
                    const Rect& source,
                    float rotation = 0.0f,
                    float alpha = 1.0f,
                    const Color& color = Color::White()) override;
    void show() override;
    void shutdown() override;

    bool shouldClose() const;

    int getWidth() const override { return m_width; }
    int getHeight() const override { return m_height; }

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

} // namespace retronomicon::opengl::graphics::renderer
