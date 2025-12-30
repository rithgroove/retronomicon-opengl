#pragma once
#include "retronomicon/graphics/color.h"

namespace retronomicon::opengl::graphics {

    /**
     * @class OpenGLColor
     * @brief OpenGL-specific color representation.
     *
     * OpenGLColor extends the engine-agnostic Color class and provides
     * convenience utilities for interoperability with OpenGL APIs.
     *
     * It preserves the same RGBA floating-point layout as the base Color
     * while allowing:
     *  - Explicit conversion from Color
     *  - Direct access to a contiguous float array suitable for OpenGL
     *
     * This class does not allocate memory and is trivially copyable.
     */
    class OpenGLColor : public retronomicon::graphics::Color {
    public:
        /**
         * @brief Inherit constructors from Color.
         *
         * Allows construction using the same parameters as the base class.
         */
        using retronomicon::graphics::Color::Color;

        /**
         * @brief Explicitly converts a generic Color into an OpenGLColor.
         *
         * Copies the RGBA components from the base Color.
         *
         * @param color Source Color.
         */
        explicit OpenGLColor(const retronomicon::graphics::Color& color) noexcept {
            // Access protected members from base class directly
            this->m_r = color.r();
            this->m_g = color.g();
            this->m_b = color.b();
            this->m_a = color.a();
        }

        /**
         * @brief Returns a pointer to the native OpenGL-compatible color array.
         *
         * The returned pointer refers to a contiguous array of four floats
         * in RGBA order.
         *
         * @return Pointer to RGBA float data.
         */
        const float* toNative() const noexcept {
            return reinterpret_cast<const float*>(&m_r);
        }

        /**
         * @brief Writes the color components into a float array.
         *
         * @param out Array of at least four floats receiving RGBA values.
         */
        void toArray(float out[4]) const noexcept {
            out[0] = m_r;
            out[1] = m_g;
            out[2] = m_b;
            out[3] = m_a;
        }
    };

} // namespace retronomicon::opengl::graphics
