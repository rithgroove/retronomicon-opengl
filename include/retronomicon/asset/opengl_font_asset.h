#pragma once

#include <vector>
#include <memory>
#include <string>

#include "retronomicon/asset/font_asset.h"

namespace retronomicon::opengl::asset {

    /**
     * @brief OpenGL-specific font asset implementation.
     *
     * This class extends the backend-agnostic `FontAsset` by:
     *  - rasterizing glyphs (via FreeType or equivalent),
     *  - packing glyph bitmaps into a single RGBA texture atlas,
     *  - computing glyph metrics and atlas coordinates,
     *  - exposing raw atlas pixels for OpenGL texture upload.
     *
     * Ownership model:
     *  - This class owns CPU-side atlas memory (`m_pixels`).
     *  - GPU texture creation is handled externally (e.g. by OpenGLTextureManager).
     */
    class OpenGLFontAsset : public retronomicon::asset::FontAsset {
    public:
        /**
         * @brief Construct an OpenGL font asset.
         *
         * @param path      Path to the font file.
         * @param pointSize Font size in points.
         */
        explicit OpenGLFontAsset(const std::string& path, int pointSize)
            : FontAsset(path, pointSize) {}

        /**
         * @brief Construct with explicit asset name.
         *
         * @param path      Path to the font file.
         * @param name      Asset identifier.
         * @param pointSize Font size in points.
         */
        OpenGLFontAsset(const std::string& path,
                        const std::string& name,
                        int pointSize)
            : FontAsset(path, name, pointSize) {}

        /**
         * @brief Destructor releases CPU-side atlas memory.
         */
        ~OpenGLFontAsset() override {
            unload();
        }

        // --------------------------------------------------------
        // Backend loading
        // --------------------------------------------------------

        /**
         * @brief Load and rasterize font glyphs, then build texture atlas.
         *
         * @return true on success.
         */
        bool load() override;

        /**
         * @brief Release glyph metrics and atlas pixel data.
         */
        void unload() override;

        /**
         * @brief Check whether the font asset is fully loaded.
         */
        bool isLoaded() const noexcept override { return m_isLoaded; }

        // --------------------------------------------------------
        // Accessors (used by OpenGLTextureManager)
        // --------------------------------------------------------

        /**
         * @brief Get raw RGBA atlas pixels.
         *
         * These pixels are intended to be uploaded to a GPU texture.
         */
        const std::vector<uint8_t>& getAtlasPixels() const noexcept { return m_pixels; }

        /**
         * @brief Get atlas width in pixels.
         */
        int getAtlasWidth() const noexcept { return m_atlasWidth; }

        /**
         * @brief Get atlas height in pixels.
         */
        int getAtlasHeight() const noexcept { return m_atlasHeight; }

        /**
         * @brief Debug string describing this font asset.
         */
        std::string to_string() const override;

    private:
        bool m_isLoaded = false; ///< True once glyphs and atlas are ready.

        // --------------------------------------------------------
        // Atlas data (CPU-side)
        // --------------------------------------------------------
        std::vector<uint8_t> m_pixels; ///< RGBA atlas pixel buffer.
        int m_atlasWidth  = 0;         ///< Atlas width in pixels.
        int m_atlasHeight = 0;         ///< Atlas height in pixels.

        // --------------------------------------------------------
        // Internal loading steps
        // --------------------------------------------------------

        /**
         * @brief Rasterize glyphs and populate glyph metrics.
         *
         * Fills:
         *  - `FontAsset::m_glyphs`
         *  - intermediate glyph bitmap buffers
         */
        bool loadGlyphs();

        /**
         * @brief Pack glyph bitmaps into a single atlas image.
         *
         * Fills:
         *  - `m_pixels`
         *  - atlas coordinates in `FontAsset::GlyphMetrics`
         */
        bool buildAtlas();
    };

} // namespace retronomicon::opengl::asset
