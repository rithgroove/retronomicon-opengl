#pragma once
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include "retronomicon/graphics/renderer/irenderer.h"

namespace retronomicon::opengl {

    class OpenGLRenderer : public retronomicon::graphics::renderer::IRenderer {
    public:
        OpenGLRenderer(int width, int height, const char* title);
        void init() override;
        void render() override;
        void shutdown() override;
        ~OpenGLRenderer() override;

    private:
        SDL_Window* window = nullptr;
        SDL_GLContext glContext = nullptr;
        int screenWidth;
        int screenHeight;
        const char* windowTitle;
    };

} // namespace retronomicon::opengl
