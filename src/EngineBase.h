#pragma once

#include "Rendering/OpenGLWrapper/DebugGL.h"

#include "GameState.h"
#include "ImGuiMenu.h"
#include "Rendering/Material.h"
#include "Rendering/BatchRenderer.h"

//////////
#include <glm/gtc/matrix_transform.hpp>

class EngineBase {
public:
    EngineBase()
        : imGuiMenu(this),
        shouldQuit(false), isInGame(false), isGamePaused(false)
    {}

    ~EngineBase() {
        // Cleanup
        imGuiMenu.CleanUp();
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

        RawImage vehicleRawImage("assets/sprites/2D-image-pack-alien-patrol/vehicle/v-b-01.png");
        vehicle.UploadToGPU(vehicleRawImage.ToPowerOfTwo());
        vehicleSprite = Sprite(0, 0, vehicleRawImage.width, vehicleRawImage.height, &vehicle);

        RawImage vehicleGarageRawImage("assets/sprites/2D-image-pack-alien-patrol/vehicle-garage/vehicle-garage.png");
        vehicleGarage.UploadToGPU(vehicleGarageRawImage.ToPowerOfTwo());
        vehicleGarageSprite = Sprite(0, 0, vehicleGarageRawImage.width, vehicleGarageRawImage.height, &vehicleGarage);
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
                        orthographicMatrix = glm::ortho(0.0f, (float)event->window.data1, 0.0f, (float)event->window.data2);
                    }
                }
            } break;

            case SDL_KEYDOWN:
            case SDL_KEYUP:
            {
                // int keyScancode = event->key.keysym.scancode;
                SDL_Keycode key = event->key.keysym.sym;
                if (key == SDL_KeyCode::SDLK_ESCAPE) {
                    // Quit();
                }
            } break;
        }

        return false;
    }

    // called before Draw
    void Update(double deltaTime) {
        if (isGamePaused) {
            return;
        }

        static float rotation = 0.1f;

        enemies.Clear();
        enemies.Add(vehicleSprite, 550, 550, -glm::radians(rotation), 1, 1);
        enemies.Add(vehicleSprite, 750, 150, glm::radians(rotation), 1, 2);
        enemies.Add(vehicleGarageSprite, 150, 50);
        enemies.Add(vehicleSprite, 500, 80);
        enemies.FinishRecording();

        rotation += 0.05 * deltaTime;
    }

    // ...
    void Draw() {
        CheckForGLErrors();

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        if (isInGame) {
            // sprites
            spriteMaterial.Bind();
            spriteMaterial.UpdateProjection(orthographicMatrix);
            spriteMaterial.UpdateModelView(glm::mat4(1.0));
            spriteMaterial.UpdateTexture(&vehicle);

            batchRenderer.Draw(enemies);

            spriteMaterial.Unbind();
        }

        // UI
        imGuiMenu.DrawImGui(window, gameState);
    }

private:
    SDL_Window* window;

    bool shouldQuit, isInGame, isGamePaused;

    glm::mat4 projectionMatrix;
    glm::mat4 orthographicMatrix;

    ImGuiMenu imGuiMenu;
    BatchRenderer batchRenderer;
    SpriteMaterial spriteMaterial;
    GameState gameState;

    // test
    TextureGPU vehicle;
    TextureGPU vehicleGarage;

    Sprite vehicleSprite;
    Sprite vehicleGarageSprite;

    BatchRecorder enemies;
};
