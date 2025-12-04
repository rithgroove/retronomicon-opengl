#pragma once

#include <memory>
#include "retronomicon/graphics/texture.h"
#include "retronomicon/manager/texture_manager.h"
#include "retronomicon/asset/image_asset.h"
#include "retronomicon/asset/font_asset.h"

namespace retronomicon::opengl::manager {

    using retronomicon::manager::TextureManager;
    using retronomicon::graphics::Texture;
    using retronomicon::asset::ImageAsset;
    using retronomicon::asset::FontAsset;

    class OpenGLTextureManager : public TextureManager {
    public:
        OpenGLTextureManager();

        std::shared_ptr<Texture> createTexture(std::shared_ptr<ImageAsset> imageAsset) override;

        // NEW OVERLOAD FOR FONT ASSET
        std::shared_ptr<Texture> createTexture(std::shared_ptr<FontAsset> fontAsset) override;
    };

} // namespace retronomicon::opengl::manager
