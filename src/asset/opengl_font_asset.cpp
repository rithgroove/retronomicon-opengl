#include "retronomicon/asset/opengl_font_asset.h"
#include <iostream>
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#include <cstring>
#include <fstream> 

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

static bool loadFile(const std::string& path, std::vector<uint8_t>& out) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) return false;

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    out.resize(size);
    return file.read(reinterpret_cast<char*>(out.data()), size).good();
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
    std::vector<uint8_t> fontBuffer;
    if (!loadFile(m_path, fontBuffer)) {
        std::cerr << "[OpenGLFontAsset] Failed to read font file: " << m_path << "\n";
        return false;
    }

    stbtt_fontinfo font;
    if (!stbtt_InitFont(&font, fontBuffer.data(), 0)) {
        std::cerr << "[OpenGLFontAsset] stbtt_InitFont failed\n";
        return false;
    }

    float scale = stbtt_ScaleForPixelHeight(&font, (float)m_pointSize);

    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&font, &ascent, &descent, &lineGap);

    m_glyphs.clear();
    m_bitmaps.clear();

    for (char c = 32; c < 127; ++c) {
        int w, h, xoff, yoff;

        unsigned char* bitmap = stbtt_GetCodepointBitmap(
            &font,
            0,
            scale,
            c,
            &w,
            &h,
            &xoff,
            &yoff
        );

        int advance, lsb;
        stbtt_GetCodepointHMetrics(&font, c, &advance, &lsb);

        GlyphMetrics gm;
        gm.width    = w;
        gm.height   = h;
        gm.advanceX = int(advance * scale);
        gm.advanceY = 0;
        gm.bearingX = xoff;
        gm.bearingY = -yoff; // IMPORTANT for Y-down coordinate system

        m_glyphs[c] = gm;

        if (w > 0 && h > 0) {
            GlyphBitmap bmp;
            bmp.width  = w;
            bmp.height = h;
            bmp.pixels.resize(w * h);

            std::memcpy(bmp.pixels.data(), bitmap, w * h);
            m_bitmaps[c] = std::move(bmp);
        }

        stbtt_FreeBitmap(bitmap, nullptr);
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
                int srcIdx = gy * bmp.width + gx;
                uint8_t coverage = bmp.pixels[srcIdx];

                int px = gm.atlasX + gx;
                int py = gm.atlasY + gy;

                int dstIdx = (py * m_atlasWidth + px) * 4;

                // White text, glyph in alpha
                m_pixels[dstIdx + 0] = 255;
                m_pixels[dstIdx + 1] = 255;
                m_pixels[dstIdx + 2] = 255;
                m_pixels[dstIdx + 3] = coverage;


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
