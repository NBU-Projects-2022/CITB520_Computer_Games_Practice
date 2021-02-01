#pragma once

#include <SDL.h>

#include "Game.h"
// #include "EngineBase.h"

class EngineBase;

class ImGuiMenu {
public:
    ImGuiMenu(EngineBase * engineBase);

    void SetupContext(SDL_Window* window, SDL_GLContext gl_context, const char * glsl_version);
    void CleanUp();

    void UpdateInput(const SDL_Event * event);

    //
    void DrawImGui(SDL_Window* window, Game & gameState);

private:
    EngineBase * engineBase;

    void DrawInGameUI(Game & gameState);
    void DrawPauseMenu();
    void DrawMainMenu();
};