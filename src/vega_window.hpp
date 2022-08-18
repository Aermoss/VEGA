#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "vega_color.hpp"
#include "vega_input.hpp"

#include <iostream>
#include <string>

namespace vega {
    class VEGAWindow {
        private:
            GLFWwindow* window;
            VEGAColor background_color;
            int width, height;
            const char* title;
            const char* icon_path;
            bool resizable, vsync, counter;
            float fps, ms;
            double current_time = 0.0;
            double previous_time = 0.0;
            unsigned int frames;

        public:
            VEGAWindow(int width, int height, const char* title, const char* icon_path, VEGAColor background_color, bool fullscreen, bool resizable, bool vsync, bool counter);
            ~VEGAWindow();

            VEGAInput* input;

            bool should_close();
            int check_errors(bool log = true);
            float get_fps();
            float get_ms();
            void maximize();
            void iconify();
            void destroy();
            void clear_color();
            void swap_buffers();
            void handle_events();
            const char* get_icon_path();
            void set_icon(const char* icon_path);
            const char* get_title();
            void set_title(const char* title);
            void update_viewport();
            void get_size(int* width, int* height);
            void set_size(int width, int height);
            VEGAColor* get_background_color();
            void set_background_color(VEGAColor background_color);
            void set_vsync(bool vsync);
            void set_counter(bool counter);
            bool get_vsync();
            bool get_counter();
            bool get_resizable();
            GLFWwindow* get();
    };
}