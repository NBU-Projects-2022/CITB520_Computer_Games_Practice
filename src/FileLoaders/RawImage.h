#pragma once

#include <cstdint>
// #include <string>
#include <type_traits>
#include <GL/gl3w.h>

enum class TextureColorFormat : uint16_t {
    NOT_SET = 0,
    RED = GL_RED,
    RG = GL_RG,
    RGB = GL_RGB,
    RGBA = GL_RGBA
};

struct RawImage {
    RawImage();
    RawImage(RawImage && other);
    RawImage(const char * path);
    ~RawImage();

    RawImage ToPowerOfTwo();

    int32_t width;
    int32_t height;
    TextureColorFormat format;

    uint8_t *data;

    void Clear();
private:
    bool isSTBLoaded;
    int32_t componentsPerPixel;
};
