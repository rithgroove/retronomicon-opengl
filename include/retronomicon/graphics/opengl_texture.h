#pragma once

#include <memory>
#include <vector>
#include <cstdint>
#include "retronomicon/graphics/texture.h"
#include "retronomicon/asset/image_asset.h"

namespace retronomicon::opengl::graphics {

    using retronomicon::graphics::Texture;
    using retronomicon::asset::ImageAsset;

    class OpenGLTexture : public Texture {
    public:
        // Existing: from ImageAsset
        OpenGLTexture(std::shared_ptr<ImageAsset> image);

        // NEW: from raw pixel buffer
        OpenGLTexture(
            const uint8_t* pixels,
            int width,
            int height,
            int channels
        );

        ~OpenGLTexture();

        int getWidth() const override;
        int getHeight() const override;
        void bind() const override;
        void unbind() const override;

    private:
        unsigned int m_textureId;
        int m_width;
        int m_height;
    };
}
