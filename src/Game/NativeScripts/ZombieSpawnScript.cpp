#include "Scripts.h"

#include "ComponentSystem/Components.h"
#include "Core/AssetManager.h"
#include "Physics/Colliders.h"

#include "Game/Game.h"

void ZombieSpawnScript::OnInit() {
    zombie = AssetManager::LoadCachedImageFile("assets/sprites/zombie.png");
    zombieSprite = Sprite(0, 0, zombie->GetWidth(), zombie->GetHeight(), zombie.get());
}

void ZombieSpawnScript::Update(float deltaTime) {
    auto & gameStateInstance = GameState::Instance();
    auto & spawnCommands = gameStateInstance.spawnCommands;
    if (!spawnCommands.empty()) {
        auto & front = spawnCommands.front();
        if (front.gameTime < gameStateInstance.totalGameTime && front.layerId == layerId) {
            SpawnZombie();
            spawnCommands.pop();
        }
    }
}

void ZombieSpawnScript::SpawnZombie() {
    auto & position = GetComponent<TransformComponent>().position;
    auto zombieEntity = CreateGameObject();
    zombieEntity->assign<TransformComponent>(position); // assign() takes arguments and passes them to the constructor
    zombieEntity->assign<RenderComponent>(CreateRef<Sprite>(zombieSprite));
    Collider * zombieBoxCollider = new BoxCollider(zombieEntity, 0, 0, (float)zombie->GetWidth(), (float)zombie->GetHeight());
    zombieBoxCollider->debugColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    zombieBoxCollider->collisionLayer = collisionLayer | CollisionLayers::ZOMBIE;
    zombieBoxCollider->collidesWithLayers = collisionLayer
        | CollisionLayers::GROUND
        | CollisionLayers::PLANT
        | CollisionLayers::LAWNMOWER
        | CollisionLayers::BULLET;
    zombieEntity->assign<ColliderComponent>(Ref<Collider>(zombieBoxCollider));
    zombieEntity->assign<NativeScriptComponent>()->Bind<ZombieScript>();
    auto rigidBody = zombieEntity->assign<RigidBodyComponent>();
    rigidBody->isKinematic = true;

    auto & gameStateInstance = GameState::Instance();
    ++gameStateInstance.spawnedZombies;
}
