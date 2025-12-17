#include "retronomicon/graphics/opengl_texture.h"
#include <glad/gl.h>
#include <stdexcept>

namespace retronomicon::opengl::graphics {

// ------------------------------------------------------------
// Shared internal function for both constructors
// ------------------------------------------------------------
static inline void uploadTexture(
    unsigned int& id,
    const uint8_t* pixels,
    int width,
    int height,
    int channels
){
    if (width <= 0 || height <= 0)
        throw std::runtime_error("OpenGLTexture: invalid dimensions");

    GLint internalFormat = (channels == 4) ? GL_RGBA8 : GL_RGB8;
    GLenum dataFormat    = (channels == 4) ? GL_RGBA  : GL_RGB;

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        internalFormat,
        width,
        height,
        0,
        dataFormat,
        GL_UNSIGNED_BYTE,
        pixels
    );

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// ------------------------------------------------------------
// Constructor: from ImageAsset (original)
// ------------------------------------------------------------
OpenGLTexture::OpenGLTexture(std::shared_ptr<ImageAsset> image)
: m_textureId(0)
, m_width(image->getWidth())
, m_height(image->getHeight())
{
    uploadTexture(
        m_textureId,
        image->getPixels().data(),
        m_width,
        m_height,
        image->getChannels()
    );
}

// ------------------------------------------------------------
// Constructor: from raw pixel buffer (new)
// ------------------------------------------------------------
OpenGLTexture::OpenGLTexture(
    const uint8_t* pixels,
    int width,
    int height,
    int channels
)
: m_textureId(0)
, m_width(width)
, m_height(height)
{
    uploadTexture(
        m_textureId,
        pixels,
        width,
        height,
        channels
    );
}

OpenGLTexture::~OpenGLTexture() {
    if (m_textureId != 0)
        glDeleteTextures(1, &m_textureId);
}

int OpenGLTexture::getWidth() const { return m_width; }
int OpenGLTexture::getHeight() const { return m_height; }

void OpenGLTexture::bind() const { glBindTexture(GL_TEXTURE_2D, m_textureId); }
void OpenGLTexture::unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

} // namespace retronomicon::opengl::graphics
