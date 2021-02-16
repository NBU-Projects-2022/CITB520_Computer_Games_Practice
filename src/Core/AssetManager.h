#pragma once

#include <map>
#include <string>

#include "Common.h"
#include "Rendering/OpenGLWrapper/TextureGPU.h"

class AssetManager {
public:

    static Ref<TextureGPU> LoadCachedImageFile(const char * path) {
        static std::map<std::string, Ref<TextureGPU>> cachedTextures;

        std::string pathStr(path);
        if (cachedTextures.find(pathStr) == cachedTextures.end()) {
            auto texture = LoadImageFile(path);
            cachedTextures.insert({ pathStr, texture });
            return texture;
        }

        return cachedTextures[pathStr];
    }

private:
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
