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

        int getR() { return r; }
        int getG() { return g; }
        int getB() { return b; }
        int getA() { return a; }

        float getRf() { return r / 255.0f; }
        float getGf() { return g / 255.0f; }
        float getBf() { return b / 255.0f; }
        float getAf() { return a / 255.0f; }

        glm::vec3 getRGB() { return glm::vec3(getR(), getG(), getB()); }
        glm::vec4 getRGBA() { return glm::vec4(getR(), getG(), getB(), getA()); }

        glm::vec3 getRGBf() { return glm::vec3(getRf(), getGf(), getBf()); }
        glm::vec4 getRGBAf() { return glm::vec4(getRf(), getGf(), getBf(), getAf()); }

        void setR() { this->r = r; }
        void setG() { this->g = g; }
        void setB() { this->b = b; }
        void setA() { this->a = a; }

        void setRf(float r) { this->r = (int) (r * 255); }
        void setGf(float g) { this->g = (int) (g * 255); }
        void setBf(float b) { this->b = (int) (b * 255); }
        void setAf(float a) { this->a = (int) (a * 255); }

        void setRGB(glm::vec3 color) {
            r = color.r;
            g = color.g;
            b = color.b;
        }

        void setRGBA(glm::vec4 color) {
            r = color.r;
            g = color.g;
            b = color.b;
            a = color.a;
        }

        void setRGBf(glm::vec3 color) {
            r = (int) (color.r * 255);
            g = (int) (color.g * 255);
            b = (int) (color.b * 255);
        }

        void setRGBAf(glm::vec4 color) {
            r = (int) (color.r * 255);
            g = (int) (color.g * 255);
            b = (int) (color.b * 255);
            a = (int) (color.a * 255);
        }
    };
}