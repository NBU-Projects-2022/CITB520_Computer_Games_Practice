#pragma once

#include "Rendering/OpenGLWrapper/DebugGL.h"

#include "GameState.h"
#include "ImGuiMenu.h"
#include "Rendering/Material.h"
#include "Rendering/BatchRenderer.h"

#include "ComponentSystem/Systems.h"
#include "Physics/Colliders.h"
#include "Game/Scripts.h"

//////////
#include <glm/gtc/matrix_transform.hpp>

class EngineBase {
public:
    EngineBase()
        : imGuiMenu(this),
        shouldQuit(false), isInGame(true), isGamePaused(false), isDebugEnabled(true)
    {}

    ~EngineBase() {
        // Cleanup
        imGuiMenu.CleanUp();
        world->destroyWorld();
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

        world = ECS::World::createWorld();
        world->registerSystem(new PhysicsSystem(-98.0f));
        world->registerSystem(new BehaviourSystem());

        // ======================
        RawImage vehicleRawImage("assets/sprites/2D-image-pack-alien-patrol/vehicle/v-b-09.png");
        vehicle.UploadToGPU(vehicleRawImage.ToPowerOfTwo());
        vehicleSprite = Sprite(0, 0, vehicleRawImage.width, vehicleRawImage.height, &vehicle);

        ECS::Entity* vehicleEntity = world->create();
        vehicleEntity->assign<PositionComponent>(500.0f, 150.0f); // assign() takes arguments and passes them to the constructor
        vehicleEntity->assign<RotationComponent>(0.0f);
        vehicleEntity->assign<RenderComponent>(CreateRef<Sprite>(vehicleSprite));
        Collider * vehicleBoxCollider = new BoxCollider(vehicleEntity, 0, 0, vehicleRawImage.width, vehicleRawImage.height);
        vehicleEntity->assign<ColliderComponent>(Ref<Collider>(vehicleBoxCollider));
        vehicleEntity->assign<BehaviourComponent>(CreateRef<VehicleBehaviourScript>());
        vehicleEntity->assign<RigidBodyComponent>();

        ECS::Entity* groundEntity = world->create();
        groundEntity->assign<PositionComponent>(0.0f, 0.0f); // assign() takes arguments and passes them to the constructor
        groundEntity->assign<RotationComponent>(0.0f);
        Collider * groundBoxCollider = new BoxCollider(groundEntity, 0, 0, 1920, 120);
        groundEntity->assign<ColliderComponent>(Ref<Collider>(groundBoxCollider));
        groundEntity->assign<RigidBodyComponent>(true);

        gameState.level.Init();
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
        gameState.level.Update(deltaTime);

        static float rotation = 0.1f;

        units.Clear();
        // units.Add(vehicleSprite, 550, 550, -glm::radians(rotation), 1, 1);
        // units.Add(vehicleSprite, 750, 150, glm::radians(rotation), 1, 2);

        world->each<PositionComponent, RotationComponent, RenderComponent>(
            [&](
                ECS::Entity* ent,
                ECS::ComponentHandle<PositionComponent> position,
                ECS::ComponentHandle<RotationComponent> rotation,
                ECS::ComponentHandle<RenderComponent> render
            ) {
                units.Add(*render->sprite, position->x, position->y, glm::radians(rotation->angle), 1.0f, 1.0f);
            });
        units.FinishRecording();

        rotation += 5 * deltaTime;
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
        spriteMaterial.UpdateTexture(&vehicle);

        spriteMaterial.UpdateIsDebug(false);
        batchRenderer.Draw(gameState.level.terrain);
        batchRenderer.Draw(units);

        if (isDebugEnabled) {
            spriteMaterial.UpdateIsDebug(true);
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            batchRenderer.Draw(gameState.level.terrain);
            batchRenderer.Draw(units);
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
    GameState gameState;

    // test
    TextureGPU vehicle;
    TextureGPU vehicleGarage;

    Sprite vehicleSprite;
    Sprite vehicleGarageSprite;

    BatchRecorder units;
};
