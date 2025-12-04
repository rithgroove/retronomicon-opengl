#pragma once

#include <vector>
#include <memory>
#include <string>

#include "retronomicon/asset/font_asset.h"

namespace retronomicon::opengl::asset {

    class OpenGLFontAsset : public retronomicon::asset::FontAsset {
    public:
        explicit OpenGLFontAsset(const std::string& path, int pointSize)
            : FontAsset(path, pointSize) {}

        OpenGLFontAsset(const std::string& path,
                        const std::string& name,
                        int pointSize)
            : FontAsset(path, name, pointSize) {}

        ~OpenGLFontAsset() override {
            unload();
        }

        // --------------------------------------------------------
        // Backend loading
        // --------------------------------------------------------
        bool load() override;
        void unload() override;
        bool isLoaded() const noexcept override { return m_isLoaded; }

        // --------------------------------------------------------
        // Accessors (used by OpenGLTextureManager)
        // --------------------------------------------------------
        const std::vector<uint8_t>& getAtlasPixels() const noexcept { return m_pixels; }
        int getAtlasWidth() const noexcept { return m_atlasWidth; }
        int getAtlasHeight() const noexcept { return m_atlasHeight; }

        std::string to_string() const override;

    private:
        bool m_isLoaded = false;

        // Atlas data
        std::vector<uint8_t> m_pixels;  // RGBA
        int m_atlasWidth = 0;
        int m_atlasHeight = 0;

        // Internal steps
        bool loadGlyphs();  // fill m_glyphs + raw bitmaps
        bool buildAtlas();  // pack bitmaps into m_pixels
    };

} // namespace retronomicon::opengl::asset
