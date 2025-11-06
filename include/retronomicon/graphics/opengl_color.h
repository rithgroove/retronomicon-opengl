#pragma once
#include "retronomicon/graphics/color.h"

namespace retronomicon::opengl::graphics {

    class OpenGLColor : public retronomicon::graphics::Color {
        public:
            using retronomicon::graphics::Color::Color; // Inherit constructors

            // Convert to OpenGL native float array
            const float* toNative() const noexcept {
                return reinterpret_cast<const float*>(&m_r);
            }

            // For convenience if you need array form
            void toArray(float out[4]) const noexcept {
                out[0] = m_r;
                out[1] = m_g;
                out[2] = m_b;
                out[3] = m_a;
            }
    };

} // namespace retronomicon::opengl::graphics
