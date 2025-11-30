#include "retronomicon/asset/opengl_font_asset.h"
#include <iostream> // temp logging

namespace retronomicon::opengl::asset {

bool OpenGLFontAsset::load() {
    if (m_isLoaded) return true;

    // Step 1: load glyph metrics (with stb_truetype or FreeType)
    if (!loadGlyphs()) {
        std::cerr << "Failed to load glyph metrics for font: " << m_path << "\n";
        return false;
    }

    // Step 2: build texture atlas
    if (!buildAtlas()) {
        std::cerr << "Failed to build texture atlas for font: " << m_path << "\n";
        return false;
    }

    m_isLoaded = true;
    return true;
}

void OpenGLFontAsset::unload() {
    if (!m_isLoaded) return;
    if (m_textureAtlas) {
        // m_textureAtlas->destroy();
        m_textureAtlas.reset();
    }
    m_isLoaded = false;
}

bool OpenGLFontAsset::loadGlyphs() {
    // Placeholder — backend decision: FreeType or stb_truetype.
    // This function should:
    // 1. load font file
    // 2. rasterize glyphs or gather metrics
    // 3. store metrics into m_glyphs[char]

    // For now, stub returns true so the engine compiles.
    return true;
}

bool OpenGLFontAsset::buildAtlas() {
    // Placeholder: dummy 32x32 white texture
    int atlasWidth = 32;
    int atlasHeight = 32;
    int channels = 4;

    std::vector<uint8_t> pixels(atlasWidth * atlasHeight * channels, 255);

    m_textureAtlas = std::make_shared<retronomicon::opengl::graphics::OpenGLTexture>(
        pixels.data(),
        atlasWidth,
        atlasHeight,
        channels
    );

    return true;
}


} // namespace retronomicon::asset
