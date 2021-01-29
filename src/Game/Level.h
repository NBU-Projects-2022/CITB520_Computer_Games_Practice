#pragma once

#include "Rendering/BatchRecorder.h"
#include "utils.h"

class Level {
public:

    Level()
        : offsetX(0)
    {}

    void Init() {
        const char * tileFiles[6] = {
            "assets/sprites/2D-image-pack-alien-patrol/tiles/tile-terrain-01-01.png",
            "assets/sprites/2D-image-pack-alien-patrol/tiles/tile-terrain-01-02.png",
            "assets/sprites/2D-image-pack-alien-patrol/tiles/tile-terrain-01-03.png",
            "assets/sprites/2D-image-pack-alien-patrol/tiles/tile-terrain-01-04.png",
            "assets/sprites/2D-image-pack-alien-patrol/tiles/tile-terrain-01-05.png",
            "assets/sprites/2D-image-pack-alien-patrol/vehicle-garage/vehicle-garage.png",
        };

        tiles.resize(sizeof(tileFiles)/sizeof(tileFiles[0]));
        for (int i = 0; i < tiles.size(); ++i) {
            RawImage rawImage(tileFiles[i]);
            auto& tile = tiles[i];
            tile.texture = CreateRef<TextureGPU>();
            tile.texture->UploadToGPU(rawImage.ToPowerOfTwo());
            tile.sprite = CreateRef<Sprite>(0, 0, rawImage.width, rawImage.height, tile.texture.get());
        }
    }

    void Reset() {
        offsetX = 0.0f;
    }

    void Update(float deltaTime) {
        auto& tileGround = tiles[0];
        auto& tileLeftSlope = tiles[2];
        auto& tileRightSlope = tiles[3];
        auto& tileHole = tiles[4];
        auto& garage = tiles[5];

        int tileSize = 120;
        int maxTileOffset = 1920 + tileSize;
        float offsetY = 0.0f;

        terrain.Clear();
        if (offsetX < maxTileOffset) {
            terrain.Add(*garage.sprite.get(), 150.0f - offsetX, offsetY + tileSize);
        }

        int prevDivOffsetPart = 0;
        for (int i = -50; i < maxTileOffset; i += tileSize) {
            int divOffsetPart = static_cast<int>(offsetX + i) / tileSize;
            int modOffsetPart = static_cast<int>(offsetX + i) % tileSize;

            if (divOffsetPart % 29 == 0) {
                terrain.Add(*tileHole.sprite.get(), i - modOffsetPart, offsetY);
            } else if (divOffsetPart % 37 == 0) {
                terrain.Add(*tileLeftSlope.sprite.get(), i - modOffsetPart, offsetY);
            } else if (prevDivOffsetPart % 29 != 0 && divOffsetPart % 37 == 1) {
                terrain.Add(*tileRightSlope.sprite.get(), i - modOffsetPart, offsetY);
            } else {
                terrain.Add(*tileGround.sprite.get(), i - modOffsetPart, offsetY);
            }

            prevDivOffsetPart = divOffsetPart;
        }
        terrain.FinishRecording();

        offsetX += 250 * deltaTime;
    }

    BatchRecorder terrain;
private:
    float offsetX;

    struct Tile {
        Ref<TextureGPU> texture;
        Ref<Sprite> sprite;
    };

    std::vector<Tile> tiles;
};
