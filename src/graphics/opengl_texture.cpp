#include "retronomicon/graphics/opengl_texture.h"
#include <glad/gl.h>
#include <stdexcept>

namespace retronomicon::opengl::graphics{
    OpenGLTexture::OpenGLTexture(std::shared_ptr<ImageAsset> image)
        : m_textureId(0), m_width(image->getWidth()), m_height(image->getHeight()) 
    {
        if (m_width <= 0 || m_height <= 0) {
            throw std::runtime_error("OpenGLTexture: invalid image dimensions");
        }

        // Generate texture
        glGenTextures(1, &m_textureId);
        glBindTexture(GL_TEXTURE_2D, m_textureId);

        // Texture parameters (basic defaults, tweak later)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  

        // Figure out pixel format
        GLint format = (image->getChannels() == 4) ? GL_RGBA : GL_RGB;

        // Upload pixel data to GPU
        glTexImage2D(
            GL_TEXTURE_2D,
            0,                  // mipmap level
            format,             // internal format
            m_width,
            m_height,
            0,                  // border
            format,             // data format
            GL_UNSIGNED_BYTE,   // data type
            image->getPixels().data()
        );

        // Optional: generate mipmaps
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    OpenGLTexture::~OpenGLTexture() {
        if (m_textureId != 0) {
            glDeleteTextures(1, &m_textureId);
        }
    }

    int OpenGLTexture::getWidth() const {
        return m_width;
    }

    int OpenGLTexture::getHeight() const {
        return m_height;
    }

    void OpenGLTexture::bind() const {
        glBindTexture(GL_TEXTURE_2D, m_textureId);
    }

    void OpenGLTexture::unbind() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}