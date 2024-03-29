#include <iostream>

#pragma warning(push, 0)
#include <GL/gl3w.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#pragma warning(pop)

#include "ImGuiMenu.h"
#include "EngineBase.h"
#include "Game/Game.h"

ImGuiMenu::ImGuiMenu(EngineBase * engineBase)
    : engineBase(engineBase)
{
    peashooterSeed = AssetManager::LoadCachedImageFile("assets/sprites/PeashooterSeed.png");
    sunflowerSeed = AssetManager::LoadCachedImageFile("assets/sprites/SunflowerSeed.png");
    wallnutSeed = AssetManager::LoadCachedImageFile("assets/sprites/WallnutSeed.png");
    tallnutSeed = AssetManager::LoadCachedImageFile("assets/sprites/TallnutSeed.png");
    shovel = AssetManager::LoadCachedImageFile("assets/sprites/Shovel.png");

    GameState::Instance().plantSeeds.push_back({ (int)PlantTypes::Peashooter, peashooterSeed });
    GameState::Instance().plantSeeds.push_back({ (int)PlantTypes::Sunflower, sunflowerSeed });
    GameState::Instance().plantSeeds.push_back({ (int)PlantTypes::Wallnut, wallnutSeed });
    GameState::Instance().plantSeeds.push_back({ (int)PlantTypes::Tallnut, tallnutSeed });
}

void ImGuiMenu::SetupContext(SDL_Window* window, SDL_GLContext gl_context, const char * glsl_version) {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    // io.ConfigViewportsNoAutoMerge = true;
    // io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.ScrollbarRounding = 0.0f;
    style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
    // style.SelectableTextAlign = ImVec2(0.5f, 0.5f);
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    main_viewport->Flags |= ImGuiViewportFlags_NoRendererClear;

    // Load Fonts
    // TODO: change font
    // io.Fonts->AddFontFromFileTTF("assets/fonts/DejaVuSans-ExtraLight.ttf", 48.0f);
    io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto-Medium.ttf", 48.0f);
    // io.Fonts->AddFontFromFileTTF("assets/fonts/Cousine-Regular.ttf", 48.0f);
    // io.Fonts->AddFontFromFileTTF("assets/fonts/DroidSans.ttf", 48.0f);
    // io.Fonts->AddFontFromFileTTF("assets/fonts/ProggyTiny.ttf", 48.0f);
    io.FontGlobalScale = 0.5f;
}

void ImGuiMenu::CleanUp() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiMenu::UpdateInput(const SDL_Event * event) {
    ImGui_ImplSDL2_ProcessEvent(event);
}

void ImGuiMenu::DrawImGui(SDL_Window* window, Game & gameState) {
    // Our state
    static bool show_demo_window = false;

    ImGuiIO& io = ImGui::GetIO();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window) {
        ImGui::ShowDemoWindow(&show_demo_window);
    }


    if (engineBase->IsGamePaused()) {
        DrawPauseMenu();
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape))) {
            engineBase->ResumeGame();
        }
    } else if (engineBase->IsInGame()) {
        DrawInGameUI(gameState);
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape))) {
            engineBase->PauseGame();
        }
    } else {
        DrawMainMenu();
    }

    // Rendering
    ImGui::Render();
    // glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        // should never get here for this project
        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }
}

class ActiveButtonFeedback {
public:
    ActiveButtonFeedback(bool isActive)
        : isActive(isActive)
    {
        if (isActive) {
            // ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
            // ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
            // ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        }
    }

    ~ActiveButtonFeedback() {
        if (isActive) {
            ImGui::PopStyleColor(3);
        }
    }

private:
    bool isActive;
};

void ImGuiMenu::DrawInGameUI(Game & game) {
    ImGuiIO& io = ImGui::GetIO();

    // Game UI
    static bool isOpen = true;
    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoDecoration
        | ImGuiWindowFlags_NoDocking
        | ImGuiWindowFlags_AlwaysAutoResize
        | ImGuiWindowFlags_NoSavedSettings
        | ImGuiWindowFlags_NoFocusOnAppearing
        | ImGuiWindowFlags_NoNav;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 work_area_pos = viewport->GetWorkPos();   // Instead of using viewport->Pos we use GetWorkPos() to avoid menu bars, if any!
    ImVec2 work_area_size = viewport->GetWorkSize();
    {
        const float DISTANCE = 10.0f;

        window_flags |= ImGuiWindowFlags_NoMove;
        // ImVec2 window_pos = ImVec2((corner & 1) ? (work_area_pos.x + work_area_size.x - DISTANCE) : (work_area_pos.x + DISTANCE), (corner & 2) ? (work_area_pos.y + work_area_size.y - DISTANCE) : (work_area_pos.y + DISTANCE));
        ImVec2 window_pos = ImVec2(work_area_pos.x + DISTANCE, work_area_pos.y + DISTANCE);
        // ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
        ImVec2 window_pos_pivot = ImVec2(0.0f, 0.0f);
        ImVec2 window_size = ImVec2(work_area_size.x - 2 * DISTANCE, 0.0f);
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        ImGui::SetNextWindowSize(window_size);
        ImGui::SetNextWindowViewport(viewport->ID);
    }

    auto & gameStateInstance = GameState::Instance();
    bool isGameOver = gameStateInstance.IsGameOver();
    if (isGameOver) {
        ImVec2 window_pos = ImVec2(work_area_pos.x + work_area_size.x / 2, work_area_pos.y + work_area_size.y / 2);
        ImVec2 window_pos_pivot = ImVec2(0.5f, 0.5f);
        ImVec2 window_size = ImVec2(0.0f, 0.0f);
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        ImGui::SetNextWindowSize(window_size);

        if (ImGui::Begin("Game over", &isGameOver, window_flags)) {
            if (gameStateInstance.DidWonGame()) {
                ImGui::Text("Game Won");
                ImGui::Separator();
            } else {
                ImGui::Text("Game Lost");
                ImGui::Separator();
            }


            if (ImGui::Button("Retry", ImVec2(350.0f, 0.0f))) {
            }

            if (ImGui::Button("Quit", ImVec2(350.0f, 0.0f))) {
                engineBase->Quit();
            }

            ImGui::End();
        }

        return;
    }

    if (ImGui::Begin("Info", &isOpen, window_flags))
    {
        ImGui::Text("Game Info");
        ImGui::Separator();

        static bool showDebugInfo = false;

        if (showDebugInfo) {
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::SameLine();
            ImGui::Text("Game objects count %d, spawned zombies %d, killed zombies %d",
                game.world->getCount(), gameStateInstance.spawnedZombies, gameStateInstance.killedZombies);
            ImGui::Separator();
        }

        if (ImGui::Button("Pause")) {
            engineBase->PauseGame();
        }

        ImGui::SameLine();
        if (!showDebugInfo && ImGui::Button("Show Debug Info")) {
            showDebugInfo = true;
        } else if (showDebugInfo &&  ImGui::Button("Hide Debug Info")) {
            showDebugInfo = false;
        }

        ImGui::SameLine();
        ImGui::Text("money %d, waves left %d, next wave %.2f",
            gameStateInstance.money,
            gameStateInstance.waveCount,
            gameStateInstance.waveTimer);

        for (auto& seed : gameStateInstance.plantSeeds)
        {
            if (ImGui::ImageButton(reinterpret_cast<void*>(seed.texture->GetId()), ImVec2{ (float)seed.texture->GetWidth(), (float)seed.texture->GetHeight() }))
            {
                if (!PlantSpawnScript::isHoldingPlant)
                {
                    PlantSpawnScript::plantType = seed.type;
                    PlantSpawnScript::shouldSpawn = true;
                }
            }

            ImGui::SameLine();
        }

        {
            ActiveButtonFeedback feedback(gameStateInstance.usingShovel);
            if (ImGui::ImageButton(reinterpret_cast<void*>(shovel->GetId()), ImVec2{ (float)shovel->GetWidth(), (float)shovel->GetHeight() }))
            {
                gameStateInstance.usingShovel = !gameStateInstance.usingShovel;
            }
        }

        ImGui::End();
    }
}

void ImGuiMenu::DrawPauseMenu() {
    static bool isOpen = true;
    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoDecoration
        | ImGuiWindowFlags_NoDocking
        | ImGuiWindowFlags_AlwaysAutoResize
        | ImGuiWindowFlags_NoSavedSettings
        | ImGuiWindowFlags_NoFocusOnAppearing
        | ImGuiWindowFlags_NoNav;

    {
        window_flags |= ImGuiWindowFlags_NoMove;
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_area_pos = viewport->GetWorkPos();   // Instead of using viewport->Pos we use GetWorkPos() to avoid menu bars, if any!
        ImVec2 work_area_size = viewport->GetWorkSize();
        ImVec2 window_pos = ImVec2(work_area_pos.x + work_area_size.x / 2, work_area_pos.y + work_area_size.y / 2);
        ImVec2 window_pos_pivot = ImVec2(0.5f, 0.5f);
        ImVec2 window_size = ImVec2(0.0f, 0.0f);
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        ImGui::SetNextWindowSize(window_size);
        ImGui::SetNextWindowViewport(viewport->ID);
    }

    ImGui::SetNextWindowBgAlpha(0.35f);
    if (ImGui::Begin("Game Paused Menu", &isOpen, window_flags))
    {
        ImGui::SetWindowFontScale(3.0f);
        ImGui::Text("Paused");
        ImGui::Separator();

        ImGui::SetWindowFontScale(2.0f);
        // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(128, 128, 128, 100));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(168, 168, 168, 100));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(200, 200, 200, 100));

        if (ImGui::Button("Resume", ImVec2(350.0f, 0.0f))) {
            engineBase->ResumeGame();
        }

        if (ImGui::Button("To Main Menu", ImVec2(350.0f, 0.0f))) {
            engineBase->QuitToMenu();
        }

        if (ImGui::Button("Quit", ImVec2(350.0f, 0.0f))) {
            engineBase->Quit();
        }

        ImGui::PopStyleColor(3);

        ImGui::End();
    }
}

void ImGuiMenu::DrawMainMenu() {
    static bool isOpen = true;
    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoDecoration
        | ImGuiWindowFlags_NoDocking
        | ImGuiWindowFlags_AlwaysAutoResize
        | ImGuiWindowFlags_NoSavedSettings
        | ImGuiWindowFlags_NoFocusOnAppearing
        | ImGuiWindowFlags_NoBackground
        | ImGuiWindowFlags_NoNav;

    {
        const float MARGIN = 10.0f;

        window_flags |= ImGuiWindowFlags_NoMove;
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_area_pos = viewport->GetWorkPos();   // Instead of using viewport->Pos we use GetWorkPos() to avoid menu bars, if any!
        ImVec2 work_area_size = viewport->GetWorkSize();
        ImVec2 window_pos = ImVec2(work_area_pos.x + MARGIN, work_area_pos.y + work_area_size.y / 2);
        ImVec2 window_pos_pivot = ImVec2(0.0f, 0.0f);
        ImVec2 window_size = ImVec2(0.0f, 0.0f);
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        ImGui::SetNextWindowSize(window_size);
        ImGui::SetNextWindowViewport(viewport->ID);
    }

    // ImGui::SetNextWindowBgAlpha(0.0f);
    if (ImGui::Begin("Main Menu", &isOpen, window_flags))
    {
        ImGui::SetWindowFontScale(3.0f);
        ImGui::Text("Plants Vs Zombies");
        ImGui::Separator();

        ImGui::SetWindowFontScale(2.0f);
        // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(128, 128, 128, 100));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(168, 168, 168, 100));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(200, 200, 200, 100));

        if (ImGui::Button("Start game", ImVec2(250.0f, 0.0f))) {
            engineBase->StartGame();
        }

        if (ImGui::Button("Quit", ImVec2(250.0f, 0.0f))) {
            engineBase->Quit();
        }

        ImGui::PopStyleColor(3);

        ImGui::End();
    }
}
