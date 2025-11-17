#pragma once

#include "retronomicon/input/input_map.h"
#include "retronomicon/input/glfw_raw_input.h"
#include "retronomicon/input/input_state.h"

namespace retronomicon::opengl::input {

    class GLFWInputMap : public retronomicon::input::InputMap {
    public:
        using InputMap::InputMap; // inherit constructors

        /**
         * @brief Update InputState based on current RawInput (GLFW backend)
         */
        void updateFromRawInput(const GLFWRawInput& raw,
                                retronomicon::input::InputState& state) const;
    };

} // namespace retronomicon::opengl::input
