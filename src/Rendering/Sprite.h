#pragma once

#include <cstdint>

#include "OpenGLWrapper/TextureGPU.h"

struct Sprite {
    Sprite() = default;

    Sprite(int16_t x, int16_t y, int16_t width, int16_t height, TextureGPU * texture)
        : x(x),
        y(y),
        width(width),
        height(height),
        texture(texture),
        uvMin(x / (float)texture->GetWidth(), y / (float)texture->GetHeight()),
        uvMax((x + width) / (float)texture->GetWidth(), (y + height) / (float)texture->GetHeight())
    {}

    int16_t x;
    int16_t y;
    int16_t width;
    int16_t height;
    TextureGPU * texture;

    glm::vec2 uvMin, uvMax;
};
