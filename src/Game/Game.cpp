#include "Game.h"

#include "ComponentSystem/Components.h"
#include "Game/NativeScripts/Scripts.h"
#include "Physics/Colliders.h"

void Game::Init() {
    auto zombieSpawn = CreateGameObject();
    zombieSpawn->assign<TransformComponent>(800.0f, 150.0f);
    zombieSpawn->assign<NativeScriptComponent>()->Bind<ZombieSpawnScript>();

    auto groundEntity = CreateGameObject();
    groundEntity->assign<TransformComponent>(0.0f, 0.0f); // assign() takes arguments and passes them to the constructor
    Collider * groundBoxCollider = new BoxCollider(groundEntity, 0, 0, 1920, 120);
    groundEntity->assign<ColliderComponent>(Ref<Collider>(groundBoxCollider));
    groundEntity->assign<RigidBodyComponent>(true);

    level.Init();
}

void Game::OnWindowResize(int newWidth, int newHeight) {
    level.OnWindowResize(newWidth, newHeight);
}

void Game::Update(float deltaTime) {
    // level.Update(deltaTime);
}
