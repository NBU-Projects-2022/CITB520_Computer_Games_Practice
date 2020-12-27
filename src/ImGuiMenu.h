#pragma once

#include <SDL.h>

#include "GameState.h"
// #include "EngineBase.h"

class EngineBase;

class ImGuiMenu {
public:
    ImGuiMenu(EngineBase * engineBase);

    void SetupContext(SDL_Window* window, SDL_GLContext gl_context, const char * glsl_version);
    void CleanUp();

    void UpdateInput(const SDL_Event * event);

    //
    void DrawImGui(SDL_Window* window, GameState & gameState);

private:
    EngineBase * engineBase;

    void DrawInGameUI(GameState & gameState);
    void DrawPauseMenu();
    void DrawMainMenu();
};
