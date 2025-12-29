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
     * @class OpenGLRenderer
     * @brief OpenGL-based implementation of the IRenderer interface.
     *
     * This renderer provides a simple 2D rendering backend using OpenGL.
     * It is responsible for:
     *  - Initializing OpenGL rendering resources (VAO, VBO, shaders)
     *  - Clearing and presenting frames
     *  - Rendering textured quads
     *  - Managing viewport dimensions
     *
     * The renderer operates on an existing GLFW window and does not
     * own the window lifecycle.
     */
    class OpenGLRenderer : public retronomicon::graphics::renderer::IRenderer {
    public:
        /**
         * @brief Constructs an OpenGL renderer.
         *
         * The renderer uses an already-created GLFW window and assumes
         * an active OpenGL context.
         *
         * @param window Pointer to an existing GLFW window.
         * @param width Initial render width in pixels.
         * @param height Initial render height in pixels.
         */
        OpenGLRenderer(GLFWwindow* window, int width, int height);

        /**
         * @brief Destroys the renderer and releases OpenGL resources.
         */
        ~OpenGLRenderer() override;

        /**
         * @brief Initializes OpenGL state and rendering resources.
         *
         * Compiles shaders, creates buffers, and prepares the renderer
         * for drawing.
         */
        void init() override;

        /**
         * @brief Clears the current frame buffer.
         *
         * Typically called at the beginning of each frame.
         */
        void clear() override;

        /**
         * @brief Presents the rendered frame.
         *
         * Flushes rendering commands and swaps buffers if required.
         */
        void show() override;

        /**
         * @brief Shuts down the renderer.
         *
         * Releases GPU resources and resets internal state.
         */
        void shutdown() override;

        /**
         * @brief Renders a texture using position, scale, and rotation.
         *
         * @param texture Texture to render.
         * @param position World-space position.
         * @param scale Scaling factor.
         * @param rotation Rotation in degrees.
         * @param alpha Alpha transparency multiplier.
         */
        void render(std::shared_ptr<Texture> texture,
                    const Vec2& position,
                    const Vec2& scale,
                    float rotation = 0.0f,
                    float alpha = 1.0f) override;

        /**
         * @brief Renders a textured quad with explicit source and target rectangles.
         *
         * Supports sprite sheets, texture atlases, and color modulation.
         *
         * @param texture Texture to render.
         * @param target Target rectangle in world or screen space.
         * @param source Source rectangle within the texture.
         * @param rotation Rotation in degrees.
         * @param alpha Alpha transparency multiplier.
         * @param color Color tint applied to the quad.
         */
        void renderQuad(std::shared_ptr<Texture> texture,
                        const Rect& target,
                        const Rect& source,
                        float rotation = 0.0f,
                        float alpha = 1.0f,
                        const Color& color = Color::White()) override;

        /**
         * @brief Gets the current render width.
         *
         * @return Width in pixels.
         */
        int getWidth() const override { return m_width; }

        /**
         * @brief Gets the current render height.
         *
         * @return Height in pixels.
         */
        int getHeight() const override { return m_height; }

        /**
         * @brief Checks whether the window should close.
         *
         * Typically used to terminate the main loop.
         *
         * @return true if a close request has been issued.
         */
        bool shouldClose() const;

    private:
        /**
         * @brief Compiles an OpenGL shader.
         *
         * @param type Shader type (e.g. GL_VERTEX_SHADER).
         * @param src GLSL shader source code.
         * @return Compiled shader object ID.
         */
        unsigned int compileShader(unsigned int type, const char* src);

        /**
         * @brief Creates a shader program from vertex and fragment shaders.
         *
         * @param vertSrc Vertex shader source code.
         * @param fragSrc Fragment shader source code.
         * @return Linked shader program ID.
         */
        unsigned int createShaderProgram(const char* vertSrc, const char* fragSrc);

        /** Current render width in pixels */
        int m_width;

        /** Current render height in pixels */
        int m_height;

        /** Associated GLFW window (not owned) */
        GLFWwindow* m_window = nullptr;

        /** Whether the renderer has been initialized */
        bool m_initialized = false;

        /** Vertex Array Object */
        unsigned int m_VAO = 0;

        /** Vertex Buffer Object */
        unsigned int m_VBO = 0;

        /** Active shader program */
        unsigned int m_shaderProgram = 0;
    };

} // namespace retronomicon::opengl::graphics::renderer
