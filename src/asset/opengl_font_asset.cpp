#include "retronomicon/asset/opengl_font_asset.h"
#include <iostream>

namespace retronomicon::opengl::asset {

bool OpenGLFontAsset::load() {
    if (m_isLoaded) return true;

    if (!loadGlyphs()) {
        std::cerr << "[OpenGLFontAsset] Failed to load glyph metrics.\n";
        return false;
    }else{
        std::cout << "[OpenGLFontAsset] Successfully load glyph metrics\n" << std::endl;
    }

    if (!buildAtlas()) {
        std::cerr << "[OpenGLFontAsset] Failed to build glyph atlas.\n";
        return false;
    }else{
        std::cout << "[OpenGLFontAsset] Successfully build glyph atlas\n" << std::endl;
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
    m_bitmaps.clear();

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
    // STUB
    for (char c = 32; c < 127; c++) {
        GlyphMetrics gm;
        gm.width    = 16;
        gm.height   = 16;
        gm.advanceX = 16;
        gm.bearingX = 0;
        gm.bearingY = 16;
        m_glyphs[c] = gm;

        GlyphBitmap bmp;
        bmp.width = 16;
        bmp.height = 16;
        bmp.pixels.resize(16 * 16, 255); // solid glyph

        m_bitmaps[c] = std::move(bmp);
    }
    return true;
}


bool OpenGLFontAsset::buildAtlas() {
    constexpr int padding = 2;
    constexpr int channels = 4;

    // 1. Estimate atlas width (simple heuristic)
    int estimatedWidth = 512;
    int x = padding;
    int y = padding;
    int rowHeight = 0;

    m_atlasWidth  = estimatedWidth;
    m_atlasHeight = padding;

    // 2. First pass: layout glyphs
    for (auto& [c, gm] : m_glyphs) {
        if (x + gm.width + padding > m_atlasWidth) {
            // new row
            x = padding;
            y += rowHeight + padding;
            rowHeight = 0;
        }

        gm.atlasX = x;
        gm.atlasY = y;

        x += gm.width + padding;
        rowHeight = std::max(rowHeight, gm.height);
    }

    m_atlasHeight = y + rowHeight + padding;

    // 3. Round atlas size to power-of-two (OpenGL-friendly)
    auto nextPOT = [](int v) {
        int p = 1;
        while (p < v) p <<= 1;
        return p;
    };

    m_atlasWidth  = nextPOT(m_atlasWidth);
    m_atlasHeight = nextPOT(m_atlasHeight);

    // 4. Allocate pixel buffer (transparent background)
    m_pixels.assign(m_atlasWidth * m_atlasHeight * channels, 0);

    // 5. Rasterize glyph bitmaps into RGBA atlas
    for (auto& [c, gm] : m_glyphs) {
        auto bmpIt = m_bitmaps.find(c);
        if (bmpIt == m_bitmaps.end())
            continue;

        const GlyphBitmap& bmp = bmpIt->second;

        for (int gy = 0; gy < bmp.height; ++gy) {
            for (int gx = 0; gx < bmp.width; ++gx) {
                int srcIdx = (bmp.height - 1 - gy) * bmp.width + gx;

                uint8_t coverage = bmp.pixels[srcIdx];

                int px = gm.atlasX + gx;
                int py = gm.atlasY + gy;

                int dstIdx = (py * m_atlasWidth + px) * 4;

                // RGBA: white text, alpha from glyph coverage
                m_pixels[dstIdx + 0] = 255;
                m_pixels[dstIdx + 1] = 0;
                m_pixels[dstIdx + 2] = 0;
                m_pixels[dstIdx + 3] = 255;

            }
        }

        // 6. Compute UVs
        gm.u0 = float(gm.atlasX) / m_atlasWidth;
        gm.v0 = float(gm.atlasY) / m_atlasHeight;
        gm.u1 = float(gm.atlasX + gm.width) / m_atlasWidth;
        gm.v1 = float(gm.atlasY + gm.height) / m_atlasHeight;
    }

    return true;
}


} // namespace retronomicon::opengl::asset
