#include "RawImage.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

RawImage::RawImage()
    : width(0), height(0), format(TextureColorFormat::NOT_SET), data(nullptr), componentsPerPixel(0), isSTBLoaded(false)
{}

RawImage::RawImage(RawImage && other)
    : width(other.width),
    height(other.height),
    format(other.format),
    data(other.data),
    componentsPerPixel(other.componentsPerPixel),
    isSTBLoaded(other.isSTBLoaded)
{
    other.width = 0;
    other.height = 0;
    other.format = TextureColorFormat::NOT_SET;
    other.data = nullptr;
    other.componentsPerPixel = 0;
    other.isSTBLoaded = false;
}

RawImage::RawImage(const char * filePath)
    : width(0), height(0), format(TextureColorFormat::NOT_SET), data(nullptr), componentsPerPixel(0), isSTBLoaded(true)
{
    //    int x,y,n;
    //    unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
    //    // ... process data if not NULL ...
    //    // ... x = width, y = height, n = # 8-bit components per pixel ...
    //    // ... replace '0' with '1'..'4' to force that many components per pixel
    //    // ... but 'n' will always be the number that it would have been if you said 0
    //    stbi_image_free(data)
    data = stbi_load(filePath, &width, &height, &componentsPerPixel, 0);
    switch (componentsPerPixel)
    {
        case 1:
            format = TextureColorFormat::RED;
            break;
        case 2:
            format = TextureColorFormat::RG;
            break;
        case 3:
            format = TextureColorFormat::RGB;
            break;
        case 4:
            format = TextureColorFormat::RGBA;
            break;
    }
}

int32_t NextPowerOfTwo(int32_t v) {
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

RawImage RawImage::ToPowerOfTwo() {
    RawImage newImage;
    newImage.width = NextPowerOfTwo(width);
    newImage.height = NextPowerOfTwo(height);
    newImage.format = format;
    newImage.data = new uint8_t[newImage.width * newImage.height * componentsPerPixel];
    int offset = 0;
    for (int y = 0; y < height; ++ y) {
        for (int x = 0; x < width; ++x) {
            for (int c = 0; c < componentsPerPixel; ++c) {
                newImage.data[(y * newImage.width + x) * componentsPerPixel + c] = data[offset++];
            }
        }
    }

    return std::move(newImage);
}

RawImage::~RawImage() {
    Clear();
}

void RawImage::Clear() {
    if ( data ) {
        if (isSTBLoaded) {
            stbi_image_free(data);
        } else {
            delete[] data;
        }

        data = nullptr;
    }

    width = 0;
    height = 0;
    format = TextureColorFormat::NOT_SET;
}
