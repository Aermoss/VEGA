#include "vega_input.hpp"

namespace vega {
    bool VEGAInput::get_key(int key) {
        return glfwGetKey(window, key);
    }

    bool VEGAInput::get_mouse_button(int button) {
        return glfwGetMouseButton(window, button);
    }

    void VEGAInput::get_mouse_pos(double* x, double* y) {
        glfwGetCursorPos(window, x, y);
    }

    void VEGAInput::set_mouse_pos(double x, double y) {
        glfwSetCursorPos(window, x, y);
    }

    void VEGAInput::set_mouse_visible(bool visible) {
        glfwSetInputMode(window, GLFW_CURSOR, !visible ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);
    }
}