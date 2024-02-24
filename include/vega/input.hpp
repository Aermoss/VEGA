#pragma once

#include <glfw/glfw3.h>

namespace vega {
    class VEGAInput {
        private:
            GLFWwindow* window;

        public:
            VEGAInput(GLFWwindow* window);
            ~VEGAInput();
            
            bool getKey(int key);
            bool getMouseButton(int button);
            void getMousePosition(double* x, double* y);
            void setMousePosition(double x, double y);
            void setMouseVisible(bool visible);
    };
}