#include "retronomicon/input/glfw_input_map.h"

namespace retronomicon::opengl::input {

    void GLFWInputMap::updateFromRawInput(const GLFWRawInput& raw, InputState& state) const {
        // Check action bindings
        for (const auto& [key, action] : getActionBindings()) {
            if (raw.isKeyPressed(key)) {
                state.setAction(action, true);
            } else {
                state.setAction(action, false);
            }
        }

        // Check axis bindings
        for (const auto& [axis, bindings] : getAxisBindings()) {
            float value = 0.0f;
            for (const auto& [key, weight] : bindings) {
                if (raw.isKeyPressed(key)) {
                    value += weight;
                }
            }
            state.setAxis(axis, value);
        }
    }

} // namespace retronomicon::input
