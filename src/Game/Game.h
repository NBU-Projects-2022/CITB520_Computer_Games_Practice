#pragma once

#include <queue>

#include <ECS.h>
ECS_TYPE_IMPLEMENTATION;

#include "Core/Common.h"
#include "Rendering/Sprite.h"
#include "Rendering/OpenGLWrapper/TextureGPU.h"

#include "PlantTypes.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 800;
const int ROWS = 5;  // how many rows the lawn has
const int COLS = 9;  // how many cols the lawn has
const int LAWN_X = 250;  // starting x of the lawn
const int LAWN_Y = 30;  // starting y of the lawn
const int PLOT_W = 80;  // width of a a single plot of land
const int PLOT_H = 100;  // height of a single plot of land

const float DEBUG_DRAW_GROUND_OFFSET = 0.0625f;
const float DRAW_LAYER_0 = 0.0f;
const float DRAW_LAYER_1 = 0.1f;
const float DRAW_LAYER_2 = 0.2f;
const float DRAW_LAYER_3 = 0.3f;
const float DRAW_LAYER_4 = 0.4f;
const float DRAW_LAYER_5 = 0.5f;
const float DRAW_LAYER_6 = 0.6f;
const float DRAW_LAYER_7 = 0.7f;
const float DRAW_LAYER_8 = 0.8f;
const float DRAW_LAYER_9 = 0.9f;
const float DRAW_LAYER_10 = 1.0f;

struct SpawnCommand {
    int zombieType = 0;
    int layerId = 0;
    float gameTime = 0.0f;
};

struct PlantSeed {
    int type;
    Ref<TextureGPU> texture;
};

class GameState {
public:
    static GameState& Instance() {
        static GameState ins;
        return ins;
    }

    int money = 125;
    int spawnedZombies = 0;
    int killedZombies = 0;

    float totalGameTime = 0.0f;

    float waveTimer = 45.0f;
    int waveCount = 4;

    std::queue<SpawnCommand> spawnCommands;
    std::vector<PlantSeed> plantSeeds;

    VecPlants plantStates;

    bool usingShovel = false;
    bool gameLost = false;

    bool IsGameOver() const {
        return gameLost || DidWonGame();
    }

    bool DidWonGame() const {
        return !waveCount && spawnCommands.empty() && spawnedZombies == killedZombies;
    }

    /*void AddPlantSeed(PlantTypes type, Ref<TextureGPU> tex)
    {
        plantSeeds.push_back({type, tex});
    }

    PlantSeed& GetPlantSeed(PlantTypes type)
    {
        for (auto& seed : plantSeeds)
        {
            if (seed.type == type)
            {
                return seed;
            }
        }
    }*/

private:
    GameState() = default;
};

class Game {
    friend class ImGuiMenu;
public:
    Game(ECS::World* world)
        : world(world)
    {
    }

    //Level level;

    void Init();

    void OnWindowResize(int newWidth, int newHeight);

    void Update(float deltaTime);

    ECS::Entity* CreateGameObject() {
        return world->create();
    }

    void DestroyGameObject(ECS::Entity* entity, bool immediate = false);

private:
    ECS::World* world;
    Ref<TextureGPU> texture;
    Ref<Sprite> sprite;
};
