#pragma once

#include "Rendering/BatchRecorder.h"
#include "Core/Common.h"
#include "Core/AssetManager.h"

class Level {
public:

    Level() = default;

    void Init() {
        const char * tileFiles[1] = {
            "assets/sprites/background1.jpg",
        };

        tiles.resize(sizeof(tileFiles)/sizeof(tileFiles[0]));
        for (int i = 0; i < tiles.size(); ++i) {
            auto& tile = tiles[i];
            tile.texture = AssetManager::LoadImageFile(tileFiles[i]);
            tile.sprite = CreateRef<Sprite>(0, 0, tile.texture->GetWidth(), tile.texture->GetHeight(), tile.texture.get());
        }

        auto& backgroundSprite = tiles[0];
        terrain.Clear();
        terrain.Add(*backgroundSprite.sprite.get(), 0, 0);
        terrain.FinishRecording();

        // add spawns here?
    }

    void OnWindowResize(int newWidth, int newHeight) {
        // auto& backgroundSprite = tiles[0];
        // // float scaleX = (float)newWidth / backgroundSprite.texture->GetWidth();
        // float scaleY = (float)newHeight / backgroundSprite.texture->GetHeight();
        // float scaleX = scaleY;
        // terrain.Clear();
        // terrain.Add(*backgroundSprite.sprite.get(), 0, 0, 0.0f, scaleX, scaleY);
        // terrain.FinishRecording();
    }

    void Reset() {
        // reset spawns here?
    }

    BatchRecorder terrain;
private:
    struct Tile {
        Ref<TextureGPU> texture;
        Ref<Sprite> sprite;
    };

    std::vector<Tile> tiles;
};
