#pragma once

#include <memory>
#include <vector>
#include <cstdint>
#include "retronomicon/graphics/texture.h"
#include "retronomicon/asset/image_asset.h"

namespace retronomicon::opengl::graphics {

    using retronomicon::graphics::Texture;
    using retronomicon::asset::ImageAsset;

    /**
     * @class OpenGLTexture
     * @brief OpenGL-backed implementation of the Texture interface.
     *
     * This class represents a GPU-resident 2D texture managed by OpenGL.
     * It is responsible for:
     *  - Uploading pixel data to the GPU
     *  - Managing the OpenGL texture object lifecycle
     *  - Binding and unbinding the texture for rendering
     *
     * OpenGLTexture can be constructed from:
     *  - An ImageAsset (asset-driven workflow)
     *  - A raw pixel buffer (procedural or runtime-generated textures)
     *
     * This class is backend-specific and should not be exposed directly
     * to gameplay code.
     */
    class OpenGLTexture : public Texture {
    public:
        /**
         * @brief Creates an OpenGL texture from an ImageAsset.
         *
         * Uploads the image data to the GPU and initializes
         * the underlying OpenGL texture object.
         *
         * @param image Shared pointer to a loaded ImageAsset.
         */
        OpenGLTexture(std::shared_ptr<ImageAsset> image);

        /**
         * @brief Creates an OpenGL texture from a raw pixel buffer.
         *
         * Intended for dynamically generated textures such as:
         *  - Font atlases
         *  - Framebuffers / render targets
         *  - Procedural textures
         *
         * The pixel format is inferred from the channel count.
         *
         * @param pixels Pointer to raw pixel data.
         * @param width Texture width in pixels.
         * @param height Texture height in pixels.
         * @param channels Number of color channels (e.g. 3 = RGB, 4 = RGBA).
         */
        OpenGLTexture(const uint8_t* pixels,
                      int width,
                      int height,
                      int channels);

        /**
         * @brief Destroys the OpenGL texture.
         *
         * Releases the GPU texture resource.
         */
        ~OpenGLTexture();

        /**
         * @brief Gets the texture width in pixels.
         *
         * @return Texture width.
         */
        int getWidth() const override;

        /**
         * @brief Gets the texture height in pixels.
         *
         * @return Texture height.
         */
        int getHeight() const override;

        /**
         * @brief Binds the texture to the active OpenGL texture unit.
         */
        void bind() const override;

        /**
         * @brief Unbinds the texture from the active OpenGL texture unit.
         */
        void unbind() const override;

    private:
        /** OpenGL texture object ID */
        unsigned int m_textureId;

        /** Texture width in pixels */
        int m_width;

        /** Texture height in pixels */
        int m_height;
    };

} // namespace retronomicon::opengl::graphics
