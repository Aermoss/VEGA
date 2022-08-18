#pragma once

#include <glm/glm.hpp>

namespace vega {
    struct VEGAColor {
        int r, g, b, a;

        VEGAColor(int r, int g, int b, int a) : r(r), g(g), b(b), a(a) {}
        VEGAColor(int r, int g, int b) : r(r), g(g), b(b), a(255) {}
        VEGAColor(int r, int g) : r(r), g(g), b(255), a(255) {}
        VEGAColor(int r) : r(r), g(255), b(255), a(255) {}

        VEGAColor(float r, float g, float b, float a) : r((int) (r * 255)), g((int) (g * 255)), b((int) (b * 255)), a((int) (a * 255)) {}
        VEGAColor(float r, float g, float b) : r((int) (r * 255)), g((int) (g * 255)), b((int) (b * 255)), a(255) {}
        VEGAColor(float r, float g) : r((int) (r * 255)), g((int) (g * 255)), b(0), a(255) {}
        VEGAColor(float r) : r((int) (r * 255)), g(0), b(0), a(255) {}

        VEGAColor() : r(0), g(0), b(0), a(255) {}

        int get_r() { return r; }
        int get_g() { return g; }
        int get_b() { return b; }
        int get_a() { return a; }

        float get_rf() { return r / 255.0f; }
        float get_gf() { return g / 255.0f; }
        float get_bf() { return b / 255.0f; }
        float get_af() { return a / 255.0f; }

        glm::vec3 get_rgb() { return glm::vec3(get_r(), get_g(), get_b()); }
        glm::vec4 get_rgba() { return glm::vec4(get_r(), get_g(), get_b(), get_a()); }

        glm::vec3 get_rgbf() { return glm::vec3(get_rf(), get_gf(), get_bf()); }
        glm::vec4 get_rgbaf() { return glm::vec4(get_rf(), get_gf(), get_bf(), get_af()); }

        void set_r() { this->r = r; }
        void set_g() { this->g = g; }
        void set_b() { this->b = b; }
        void set_a() { this->a = a; }

        void set_rf(float r) { this->r = (int) (r * 255); }
        void set_gf(float g) { this->g = (int) (g * 255); }
        void set_bf(float b) { this->b = (int) (b * 255); }
        void set_af(float a) { this->a = (int) (a * 255); }

        void set_rgb(glm::vec3 color) {
            r = color.r;
            g = color.g;
            b = color.b;
        }

        void set_rgba(glm::vec4 color) {
            r = color.r;
            g = color.g;
            b = color.b;
            a = color.a;
        }

        void set_rgbf(glm::vec3 color) {
            r = (int) (color.r * 255);
            g = (int) (color.g * 255);
            b = (int) (color.b * 255);
        }

        void set_rgbaf(glm::vec4 color) {
            r = (int) (color.r * 255);
            g = (int) (color.g * 255);
            b = (int) (color.b * 255);
            a = (int) (color.a * 255);
        }
    };
}