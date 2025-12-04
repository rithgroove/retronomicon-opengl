#include "retronomicon/manager/opengl_texture_manager.h"
#include "retronomicon/graphics/opengl_texture.h"
#include "retronomicon/asset/opengl_font_asset.h"

namespace retronomicon::opengl::manager {

using retronomicon::opengl::graphics::OpenGLTexture;

OpenGLTextureManager::OpenGLTextureManager() {}

std::shared_ptr<Texture>
OpenGLTextureManager::createTexture(std::shared_ptr<ImageAsset> imageAsset) {
    return std::make_shared<OpenGLTexture>(imageAsset);
}

// ------------------------------------------------------------
// NEW: Create a texture from a FontAsset (font atlas)
// ------------------------------------------------------------
std::shared_ptr<Texture>
OpenGLTextureManager::createTexture(std::shared_ptr<FontAsset> fontAsset) 
{
    // Downcast to backend font object
    auto glFont = std::dynamic_pointer_cast<retronomicon::opengl::asset::OpenGLFontAsset>(fontAsset);
    if (!glFont) {
        std::cerr << "[OpenGLTextureManager] ERROR: FontAsset is not OpenGLFontAsset.\n";
        return nullptr;
    }

    const auto& pixels = glFont->getAtlasPixels();
    int width = glFont->getAtlasWidth();
    int height = glFont->getAtlasHeight();

    if (pixels.empty() || width == 0 || height == 0) {
        std::cerr << "[OpenGLTextureManager] ERROR: OpenGLFontAsset has no atlas.\n";
        return nullptr;
    }

    // Uses the raw-buffer constructor we added earlier
    return std::make_shared<OpenGLTexture>(
        pixels.data(),
        width,
        height,
        4 // RGBA atlas
    );
}

} // namespace retronomicon::opengl::manager
