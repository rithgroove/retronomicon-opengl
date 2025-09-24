#pragma once

#include <memory>
#include "retronomicon/graphics/texture.h"
#include "retronomicon/asset/image_asset.h"

namespace retronomicon::opengl::graphics{
    using retronomicon::graphics::Texture;
    class OpenGLTexture : public Texture {
    public:
        OpenGLTexture(const retronomicon::asset::ImageAsset& image);
        ~OpenGLTexture();

        int getWidth() const override;
        int getHeight() const override;
        void bind() const override;
        void unbind() const override;

    private:
        unsigned int m_textureId; // GLuint
        int m_width;
        int m_height;
    };
}
