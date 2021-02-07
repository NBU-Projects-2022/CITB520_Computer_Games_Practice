#pragma once

#include <ECS.h>
ECS_TYPE_IMPLEMENTATION;

#include "Level.h"
#include <SDL.h>

class Game {
    friend class ImGuiMenu;
public:
    static SDL_Event event;

    Game(ECS::World* world)
        : money(0)
        //world(world)
    {
        this->world = world;
    }

    int money;

    Level level;

    void Init();

    void OnWindowResize(int newWidth, int newHeight);

    void Update(float deltaTime);

    static ECS::Entity* CreateGameObject() {
        return world->create();
    }

    static ECS::World* world;
private:
};
