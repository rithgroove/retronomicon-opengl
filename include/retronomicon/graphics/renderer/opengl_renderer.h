#pragma once
#include "retronomicon/graphics/renderer/i_renderer.h"
#include <SDL.h>

namespace retronomicon::opengl {

    class OpenGLRenderer : public retronomicon::graphics::renderer::IRenderer {
    public:
        OpenGLRenderer(int width, int height, const char* title);
        void init() override;
        void render() override;
        void shutdown() override;
        ~OpenGLRenderer() override;

    private:
        struct Impl;      // forward declaration
        Impl* impl;       // opaque pointer to SDL/OpenGL stuff
    };

} // namespace retronomicon::opengl
