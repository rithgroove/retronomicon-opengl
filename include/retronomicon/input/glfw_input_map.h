#pragma once

#include "retronomicon/input/input_map.h"
#include "retronomicon/input/glfw_raw_input.h"
#include "retronomicon/input/input_state.h"
#include <GLFW/glfw3.h>

namespace retronomicon::opengl::input {
    using retronomicon::input::InputMap;
    using retronomicon::input::InputState;

    class GLFWInputMap : public InputMap {
    public:
        using InputMap::InputMap; // inherit constructors

        /**
         * @brief Update InputState using current RawInput (GLFW)
         */
        void updateFromRawInput(const GLFWRawInput& raw, InputState& state) const;
    };

} // namespace retronomicon::input
