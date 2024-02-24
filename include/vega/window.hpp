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
            std::string title, iconPath;
            int32_t width, height;
            bool resizable, vsync;
            float lastTime;

        public:
            VEGAInput* input;
            float currentTime, deltaTime;

            VEGAWindow(int32_t width, int32_t height, const char* title, const char* iconPath = "res/icon/icon.png", \
                VEGAColor backgroundColor = {0, 0, 0, 255}, bool fullscreen = false, bool resizable = true, bool vsync = true);

            ~VEGAWindow();

            bool shouldClose();
            int32_t checkErrors(bool log = true);
            void destroy();
            void clear();
            void swapBuffers();
            void pollEvents();
            void setIcon(const char* filePath);
            const std::string& getIconPath();
            void setTitle(const char* title);
            const std::string& getTitle();
            void setSize(int32_t width, int32_t height);
            void getSize(int32_t* width, int32_t* height);
            void setBackgroundColor(VEGAColor background_color);
            VEGAColor* getBackgroundColor();
            void setVsync(bool vsync);
            bool getVsync();
            GLFWwindow* get();
    };
}