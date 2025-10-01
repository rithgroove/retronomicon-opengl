#pragma once

#include "retronomicon/input/raw_input.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <unordered_map>

namespace retronomicon::input {

    class GLFWRawInput : public RawInput {
    public:
        explicit GLFWRawInput(GLFWwindow* window);
        ~GLFWRawInput() override = default;

        void poll() override;
        void clear() override;

        const std::vector<std::string>& getEvents() const override;

        bool isKeyPressed(int keyCode) const override;

        int getMouseX() const override;
        int getMouseY() const override;
        unsigned int getMouseButtons() const override;

    private:
        GLFWwindow* m_window;
        std::vector<std::string> m_events;
        mutable double m_mouseX;
        mutable double m_mouseY;
        mutable unsigned int m_mouseButtons;
    };

} // namespace retronomicon::input
