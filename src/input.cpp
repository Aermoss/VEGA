#include <vega/input.hpp>

namespace vega {
    VEGAInput::VEGAInput(GLFWwindow* window) : window(window) {
    
    }
    
    VEGAInput::~VEGAInput() {

    }

    bool VEGAInput::getKey(int key) {
        return glfwGetKey(window, key);
    }

    bool VEGAInput::getMouseButton(int button) {
        return glfwGetMouseButton(window, button);
    }

    void VEGAInput::getMousePosition(double* x, double* y) {
        glfwGetCursorPos(window, x, y);
    }

    void VEGAInput::setMousePosition(double x, double y) {
        glfwSetCursorPos(window, x, y);
    }

    void VEGAInput::setMouseVisible(bool visible) {
        glfwSetInputMode(window, GLFW_CURSOR, !visible ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);
    }
}