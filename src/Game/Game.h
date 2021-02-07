#pragma once

#include <ECS.h>
ECS_TYPE_IMPLEMENTATION;

#include "Level.h"
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 800;

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

    Level level;

    void Init();

    void OnWindowResize(int newWidth, int newHeight);

    void Update(float deltaTime);

    ECS::Entity* CreateGameObject() {
        return world->create();
    }

    void DestroyGameObject(ECS::Entity* entity, bool immediate = false);

private:
    ECS::World* world;
};
