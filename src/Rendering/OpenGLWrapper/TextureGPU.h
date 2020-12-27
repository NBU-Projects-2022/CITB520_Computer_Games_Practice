#pragma once

#include <stdint.h>

#include <FileLoaders/RawImage.h>

class TextureGPU {
public:
    TextureGPU();
    ~TextureGPU();

    uint32_t GetId() const;

    virtual bool UploadToGPU( const RawImage &rawTexture, TextureColorFormat internalFormat = TextureColorFormat::NOT_SET);

    virtual void Delete();

    void Bind();
    void Unbind();

    int32_t GetWidth();
    int32_t GetHeight();

protected:
    uint32_t textureId;
    int32_t width, height;
};
