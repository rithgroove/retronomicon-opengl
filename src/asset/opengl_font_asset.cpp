#include "retronomicon/asset/opengl_font_asset.h"
#include <iostream>

namespace retronomicon::opengl::asset {

bool OpenGLFontAsset::load() {
    if (m_isLoaded) return true;

    if (!loadGlyphs()) {
        std::cerr << "[OpenGLFontAsset] Failed to load glyph metrics.\n";
        return false;
    }

    if (!buildAtlas()) {
        std::cerr << "[OpenGLFontAsset] Failed to build glyph atlas.\n";
        return false;
    }

    m_isLoaded = true;
    return true;
}

void OpenGLFontAsset::unload() {
    if (!m_isLoaded) return;

    m_pixels.clear();
    m_pixels.shrink_to_fit();
    m_atlasWidth = 0;
    m_atlasHeight = 0;

    m_glyphs.clear();

    m_isLoaded = false;
}

std::string OpenGLFontAsset::to_string() const {
    return "OpenGLFontAsset(name=" + m_name +
           ", path=" + m_path +
           ", pointSize=" + std::to_string(m_pointSize) +
           ", atlas=" + std::to_string(m_atlasWidth) + "x" +
                        std::to_string(m_atlasHeight) +
           ")";
}

bool OpenGLFontAsset::loadGlyphs() {
    // ********** STUB **********
    // Will be replaced by stb_truetype integration.
    // This stub creates dummy glyphs so the engine runs.

    for (char c = 32; c < 127; c++) {
        GlyphMetrics gm;
        gm.width    = 16;
        gm.height   = 16;
        gm.advanceX = 16;
        gm.bearingX = 0;
        gm.bearingY = 16;
        m_glyphs[c] = gm;
    }

    return true;
}

bool OpenGLFontAsset::buildAtlas() {
    // ********** STUB **********
    // Generates a dummy 256x256 white atlas.
    m_atlasWidth = 256;
    m_atlasHeight = 256;

    int channels = 4;
    m_pixels.resize(m_atlasWidth * m_atlasHeight * channels, 255);

    // Put glyphs into grid layout for now
    int cellSize = 16;
    int cols = m_atlasWidth / cellSize;

    int i = 0;
    for (auto& [c, gm] : m_glyphs) {
        int cx = (i % cols) * cellSize;
        int cy = (i / cols) * cellSize;

        gm.atlasX = cx;
        gm.atlasY = cy;

        gm.u0 = float(cx) / m_atlasWidth;
        gm.v0 = float(cy) / m_atlasHeight;
        gm.u1 = float(cx + gm.width) / m_atlasWidth;
        gm.v1 = float(cy + gm.height) / m_atlasHeight;

        i++;
    }

    return true;
}

} // namespace retronomicon::opengl::asset
