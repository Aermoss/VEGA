#pragma once

#include <GLFW/glfw3.h>

namespace vega {
    class VEGAInput {
        private:
            GLFWwindow* window;

        public:
            VEGAInput(GLFWwindow* window) : window(window) {};
            
            bool get_key(int key);
            bool get_mouse_button(int button);
            void get_mouse_pos(double* x, double* y);
            void set_mouse_pos(double x, double y);
            void set_mouse_visible(bool visible);
    };
}