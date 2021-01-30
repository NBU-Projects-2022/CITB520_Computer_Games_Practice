#pragma once

#include "Common.h"
#include "Rendering/OpenGLWrapper/TextureGPU.h"

class AssetManager {
public:

    static Ref<TextureGPU> LoadImageFile(const char * path) {
        RawImage rawImage(path);
        auto texture = CreateRef<TextureGPU>();
        texture->UploadToGPU(rawImage); // .ToPowerOfTwo());
        return texture;
    }

    static Ref<TextureGPU> LoadImageFile(const std::string & path) {
        return LoadImageFile(path.c_str());
    }
};
