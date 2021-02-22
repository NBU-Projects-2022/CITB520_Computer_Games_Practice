#pragma once

#include <cstdint>

#include <glm/glm.hpp>
#include "OpenGLWrapper/TextureGPU.h"

class Sprite {
public:
    Sprite() = default;
    Sprite(const Sprite & other)
        : x(other.x),
        y(other.y),
        width(other.width),
        height(other.height),
        texture(other.texture),
        drawOffset(other.drawOffset),
        uvMin(other.uvMin),
        uvMax(other.uvMax),
        color(other.color)
    {
        // DEBUG
        // static int colorIndex = 15;
        // color = generateColor(++colorIndex);
    }

    Sprite(int16_t x, int16_t y, int16_t width, int16_t height, TextureGPU * texture, glm::vec2 drawOffset = glm::vec2(0.0f))
        : x(x),
        y(y),
        width(width),
        height(height),
        texture(texture),
        drawOffset(drawOffset),
        uvMin(x / (float)texture->GetWidth(), y / (float)texture->GetHeight()),
        uvMax((x + width) / (float)texture->GetWidth(), (y + height) / (float)texture->GetHeight()),
        color(1.0f)
    {
        // DEBUG
        // static int colorIndex = 15;
        // color = generateColor(++colorIndex);
    }

    int16_t x;
    int16_t y;
    int16_t width;
    int16_t height;
    TextureGPU * texture;
    glm::vec4 color;

    glm::vec2 drawOffset;
    glm::vec2 uvMin, uvMax;

private:
    glm::vec4 generateColor(int index) {
        glm::vec4 rgba(1.0f);
        const double PHI = (1 + sqrt(5))/2;
        double phiIndex = index * PHI;
        double n = phiIndex - glm::floor(phiIndex);
        rgba.r = n;
        n = 2 * phiIndex - glm::floor(2*phiIndex);
        rgba.g = n;
        n = 3 * phiIndex - glm::floor(3*phiIndex);
        rgba.b = n;
        return rgba;
    }
};
