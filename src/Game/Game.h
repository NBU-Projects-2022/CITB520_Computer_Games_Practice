#pragma once

#include <ECS.h>
ECS_TYPE_IMPLEMENTATION;

#include "Core/Common.h"
#include "Rendering/Sprite.h"
#include "Rendering/OpenGLWrapper/TextureGPU.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 800;

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

class GameState {
public:
    static GameState& Instance() {
        static GameState ins;
        return ins;
    }

    int money = 0;

private:
    GameState() = default;
};

class Game {
    friend class ImGuiMenu;
public:
    Game(ECS::World* world)
        : world(world)
    {}

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
