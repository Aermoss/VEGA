#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "color.hpp"
#include "input.hpp"

#include <iostream>
#include <cstdint>
#include <string>

namespace vega {
    class VEGAWindow {
        private:
            GLFWwindow* window;
            VEGAColor backgroundColor;
            bool resizable, vsync, counter;
            double deltaTime, currentTime, lastTime;
            std::string title, iconPath;
            int32_t width, height;

        public:
            VEGAWindow(int width, int height, const char* title, const char* iconPath = "res/icon/icon.png", \
                VEGAColor backgroundColor = {0, 0, 0, 255}, bool fullscreen = false, bool resizable = true, bool vsync = true);

            ~VEGAWindow();

            VEGAInput* input;

            bool shouldClose();
            int checkErrors(bool log = true);
            void destroy();
            void clear();
            void swapBuffers();
            void pollEvents();
            void setIcon(const char* filePath);
            const std::string& getIconPath();
            void setTitle(const char* title);
            const std::string& getTitle();
            void setSize(int width, int height);
            void getSize(int* width, int* height);
            void setBackgroundColor(VEGAColor background_color);
            VEGAColor* getBackgroundColor();
            void setVsync(bool vsync);
            bool getVsync();
            GLFWwindow* get();
    };
}