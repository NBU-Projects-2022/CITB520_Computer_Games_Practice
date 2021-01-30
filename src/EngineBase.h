#pragma once

#include "Rendering/OpenGLWrapper/DebugGL.h"

#include "ComponentSystem/Systems.h"
#include "Core/AssetManager.h"
#include "Game/Game.h"
#include "Game/ImGuiMenu.h"
#include "Rendering/Material.h"
#include "Rendering/BatchRenderer.h"

//////////
#include <glm/gtc/matrix_transform.hpp>

class EngineBase {
public:
    EngineBase()
        : imGuiMenu(this),
        shouldQuit(false), isInGame(true), isGamePaused(false), isDebugEnabled(false),
        world(ECS::World::createWorld()),
        gameState(world)
    {}

    ~EngineBase() {
        // Cleanup
        imGuiMenu.CleanUp();
        world->destroyWorld();
    }

    void PauseGame() {
        isGamePaused = true;
    }

    void ResumeGame() {
        isGamePaused = false;
    }

    bool IsGamePaused() {
        return isGamePaused;
    }

    void StartGame() {
        if (isInGame) {
            return;
        }

        gameState.Init();
        isInGame = true;
    }

    void QuitToMenu() {
        isInGame = false;
        isGamePaused = false;
    }

    bool IsInGame() {
        return isInGame;
    }

    void Quit() {
        shouldQuit = true;
    }

    bool ShouldQuit() {
        return shouldQuit;
    }

    // called on startup
    void Init(SDL_Window* window, SDL_GLContext gl_context, const char * glsl_version) {
        EnableOpenGLDebugging();

        // glClearColor(0.0f, 0.0f, 0.0f, 1.00f);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        imGuiMenu.SetupContext(window, gl_context, glsl_version);
        this->window = window;

        int w, h;
        SDL_GetWindowSize(window, &w, &h);
        orthographicMatrix = glm::ortho(0.0f, (float)w, 0.0f, (float)h);

        world->registerSystem(new PhysicsSystem(-98.0f));
        world->registerSystem(new NativeScriptSystem());

        // ======================
        // for faster development
        // to start a game when starting the application
        if (isInGame) {
            gameState.Init();
        }
    }

    bool UpdateInput(const SDL_Event * event) {
        imGuiMenu.UpdateInput(event);

        switch (event->type) {
            case SDL_WINDOWEVENT: {
                Uint8 window_event = event->window.event;
                if (window_event == SDL_WINDOWEVENT_RESIZED) {
                    // in case ImGuiConfigFlags_ViewportsEnable is set
                    ImGuiViewport* main_viewport = ImGui::GetMainViewport();
                    ImGuiViewport* viewport = ImGui::FindViewportByPlatformHandle((void*)SDL_GetWindowFromID(event->window.windowID));
                    if (main_viewport == viewport)
                    {
                        const float TARGET_HEIGHT = 600.0f;
                        const float TARGET_RATIO = event->window.data2/TARGET_HEIGHT;
                        // background size 1400x600
                        // screen size 1080x600
                        gameState.OnWindowResize(event->window.data1, event->window.data2);
                        orthographicMatrix = glm::ortho(0.0f, (float)event->window.data1 / TARGET_RATIO, 0.0f, (float)event->window.data2 / TARGET_RATIO);
                    }
                }
            } break;

            case SDL_KEYDOWN:
                break;
            case SDL_KEYUP:
            {
                // int keyScancode = event->key.keysym.scancode;
                SDL_Keycode key = event->key.keysym.sym;
                if (key == SDL_KeyCode::SDLK_ESCAPE) {
                    Quit();
                }

                if (key == SDL_KeyCode::SDLK_F1) {
                    isDebugEnabled = !isDebugEnabled;
                }
            } break;
        }

        return false;
    }

    // called before Draw
    void Update(double deltaTime) {
        if (!isInGame || isGamePaused) {
            return;
        }

        world->tick(deltaTime);
        gameState.Update(deltaTime);

        entitySprites.Clear();
        world->each<TransformComponent, RenderComponent>(
            [&](
                ECS::Entity* ent,
                ECS::ComponentHandle<TransformComponent> transform,
                ECS::ComponentHandle<RenderComponent> render
            ) {
                entitySprites.Add(*render->sprite,
                    transform->position,
                    glm::radians(transform->rotation),
                    transform->scale);
            });
        entitySprites.FinishRecording();
    }

    // ...
    void Draw() {
        CheckForGLErrors();

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // sprites
        spriteMaterial.Bind();
        spriteMaterial.UpdateProjection(orthographicMatrix);
        spriteMaterial.UpdateModelView(glm::mat4(1.0));

        spriteMaterial.UpdateIsDebug(false);
        batchRenderer.Draw(gameState.level.terrain);
        batchRenderer.Draw(entitySprites);

        if (isDebugEnabled) {
            spriteMaterial.UpdateIsDebug(true);
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            batchRenderer.Draw(gameState.level.terrain);
            batchRenderer.Draw(entitySprites);
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }

        spriteMaterial.Unbind();

        // UI
        imGuiMenu.DrawImGui(window, gameState);
    }

private:
    SDL_Window* window;
    ECS::World* world;

    bool shouldQuit, isInGame, isGamePaused, isDebugEnabled;

    glm::mat4 projectionMatrix;
    glm::mat4 orthographicMatrix;

    ImGuiMenu imGuiMenu;
    BatchRenderer batchRenderer;
    SpriteMaterial spriteMaterial;

    Game gameState;

    BatchRecorder entitySprites;
};
