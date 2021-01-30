#pragma once

#include <ECS.h>
ECS_TYPE_IMPLEMENTATION;

#include "Level.h"

class Game {
    friend class ImGuiMenu;
public:
    Game(ECS::World* world)
        : money(0),
        world(world)
    {}

    int money;

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
