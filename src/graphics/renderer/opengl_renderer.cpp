#include "retronomicon/graphics/renderer/opengl_renderer.h"
#include "retronomicon/graphics/opengl_texture.h"

#include <iostream>
#include <stdexcept>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace retronomicon::opengl::graphics::renderer {

    OpenGLRenderer::OpenGLRenderer(const std::string& title, int width, int height)
        : m_title(title), m_width(width), m_height(height) {}

    OpenGLRenderer::~OpenGLRenderer() {
        shutdown();
    }

    void OpenGLRenderer::init() {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        // Request OpenGL 3.3 Core
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
        if (!m_window) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(m_window);

        if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
            throw std::runtime_error("Failed to initialize GLAD");
        }

        glViewport(0, 0, m_width, m_height);
        glClearColor(0.1f, 0.1f, 0.3f, 1.0f);

        // Enable blending for alpha
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Simple shader sources
        const char* vertexSrc = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        layout (location = 1) in vec2 aTexCoord;

        uniform mat4 uProjection;
        uniform mat4 uTransform;

        out vec2 TexCoord;

        void main() {
            gl_Position = uProjection * uTransform * vec4(aPos, 0.0, 1.0);
            TexCoord = aTexCoord;
        }
        )";

        const char* fragmentSrc = R"(
        #version 330 core
        in vec2 TexCoord;
        out vec4 FragColor;

        uniform sampler2D uTexture;
        uniform float uAlpha;

        void main() {
            vec4 texColor = texture(uTexture, TexCoord);
            FragColor = vec4(texColor.rgb, texColor.a * uAlpha);
        }
        )";

        m_shaderProgram = createShaderProgram(vertexSrc, fragmentSrc);

        // Quad vertex setup
        float vertices[] = {
            // pos      // tex
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f
        };

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        // Set orthographic projection
        glm::mat4 projection = glm::ortho(0.0f, (float)m_width, (float)m_height, 0.0f, -1.0f, 1.0f);
        glUseProgram(m_shaderProgram);
        GLint projLoc = glGetUniformLocation(m_shaderProgram, "uProjection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

        m_initialized = true;

        std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    }

    void OpenGLRenderer::clear() {
        if (!m_initialized) return;
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void OpenGLRenderer::render(std::shared_ptr<Texture> texture,
                                const Vec2& position,
                                const Vec2& scale,
                                float rotation,
                                float alpha) {

        if (!m_initialized || !texture) return;
        
        auto glTex = std::dynamic_pointer_cast<retronomicon::opengl::graphics::OpenGLTexture>(texture);
        if (!glTex) {
            std::cerr << "Render: Texture is not an OpenGLTexture instance!" << std::endl;
            return;
        }

        glUseProgram(m_shaderProgram);

        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(position.x, position.y, 0.0f));
        transform = glm::translate(transform, glm::vec3(0.5f * scale.x, 0.5f * scale.y, 0.0f));
        transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0, 0.0, 1.0));
        transform = glm::translate(transform, glm::vec3(-0.5f * scale.x, -0.5f * scale.y, 0.0f));
        transform = glm::scale(transform, glm::vec3(scale.x, scale.y, 1.0f));

        GLint uTransformLoc = glGetUniformLocation(m_shaderProgram, "uTransform");
        GLint uAlphaLoc = glGetUniformLocation(m_shaderProgram, "uAlpha");
        GLint uTextureLoc = glGetUniformLocation(m_shaderProgram, "uTexture");

        glUniformMatrix4fv(uTransformLoc, 1, GL_FALSE, &transform[0][0]);
        glUniform1f(uAlphaLoc, alpha);
        glUniform1i(uTextureLoc, 0);

        glActiveTexture(GL_TEXTURE0);
        glTex->bind();

        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glTex->unbind();
    }

    void OpenGLRenderer::show() {
        if (!m_initialized) return;
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    void OpenGLRenderer::shutdown() {
        if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
        if (m_VBO) glDeleteBuffers(1, &m_VBO);
        if (m_shaderProgram) glDeleteProgram(m_shaderProgram);

        if (m_window) {
            glfwDestroyWindow(m_window);
            m_window = nullptr;
        }

        if (m_initialized) {
            glfwTerminate();
            m_initialized = false;
        }
    }

    bool OpenGLRenderer::shouldClose() const {
        return m_window ? glfwWindowShouldClose(m_window) : true;
    }

    unsigned int OpenGLRenderer::compileShader(unsigned int type, const char* src) {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char log[512];
            glGetShaderInfoLog(shader, 512, nullptr, log);
            throw std::runtime_error(std::string("Shader compile error: ") + log);
        }

        return shader;
    }

    unsigned int OpenGLRenderer::createShaderProgram(const char* vertSrc, const char* fragSrc) {
        GLuint vs = compileShader(GL_VERTEX_SHADER, vertSrc);
        GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragSrc);
        GLuint prog = glCreateProgram();

        glAttachShader(prog, vs);
        glAttachShader(prog, fs);
        glLinkProgram(prog);

        GLint success;
        glGetProgramiv(prog, GL_LINK_STATUS, &success);
        if (!success) {
            char log[512];
            glGetProgramInfoLog(prog, 512, nullptr, log);
            throw std::runtime_error(std::string("Shader link error: ") + log);
        }

        glDeleteShader(vs);
        glDeleteShader(fs);
        return prog;
    }

} // namespace retronomicon::opengl::graphics::renderer
