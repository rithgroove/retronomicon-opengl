#pragma once

#include <memory>
#include "retronomicon/asset/font_asset.h"
#include "retronomicon/graphics/opengl_texture.h"

namespace retronomicon::opengl::asset {
    using retronomicon::asset::FontAsset;
    using retronomicon::opengl::graphics::OpenGLTexture;
    /**
     * @brief OpenGL-specific implementation of a FontAsset.
     *        Uses a texture atlas for glyph rendering.
     */
    class OpenGLFontAsset : public FontAsset {
    public:
        explicit OpenGLFontAsset(const std::string& path, int pointSize)
            : FontAsset(path, pointSize) {}

        OpenGLFontAsset(const std::string& path,
                        const std::string& name,
                        int pointSize)
            : FontAsset(path, name, pointSize) {}

        virtual ~OpenGLFontAsset() {
            unload();
        }

        /***************************** Load / Unload *****************************/
        bool load() override;
        void unload() override;
        bool isLoaded() const noexcept override { return m_isLoaded; }

        /***************************** Getters *****************************/
        std::shared_ptr<OpenGLTexture> getTextureAtlas() const noexcept {
            return m_textureAtlas;
        }

        std::string to_string() const override {
            return "OpenGLFontAsset(name=" + m_name +
                   ", path=" + m_path +
                   ", size=" + std::to_string(m_pointSize) + ")";
        }

    private:
        bool m_isLoaded = false;
        std::shared_ptr<OpenGLTexture> m_textureAtlas;

        bool buildAtlas();     // internal rasterization + texture build
        bool loadGlyphs();     // call FreeType/stb to populate m_glyphs
    };

} // namespace retronomicon::asset
