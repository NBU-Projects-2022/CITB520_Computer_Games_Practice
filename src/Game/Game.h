#pragma once

#include <ECS.h>
ECS_TYPE_IMPLEMENTATION;

#include "Level.h"

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

private:
    ECS::World* world;
};
