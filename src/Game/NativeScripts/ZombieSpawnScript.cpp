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
    // TODO: spawn zombies
    nextWaveIn -= deltaTime;
    if (nextWaveIn < 0) {
        auto & position = GetComponent<TransformComponent>().position;
        auto zombieEntity = CreateGameObject();
        zombieEntity->assign<TransformComponent>(position); // assign() takes arguments and passes them to the constructor
        zombieEntity->assign<RenderComponent>(CreateRef<Sprite>(zombieSprite));
        Collider * zombieBoxCollider = new BoxCollider(zombieEntity, 0, 0, (float)zombie->GetWidth(), (float)zombie->GetHeight());
        zombieBoxCollider->collisionLayer = collisionLayer | CollisionLayers::ZOMBIE;
        zombieBoxCollider->collidesWithLayers = collisionLayer
            | CollisionLayers::GROUND
            | CollisionLayers::PLANT
            | CollisionLayers::PROJECTILE;
        zombieEntity->assign<ColliderComponent>(Ref<Collider>(zombieBoxCollider));
        zombieEntity->assign<NativeScriptComponent>()->Bind<ZombieScript>();
        auto rigidBody = zombieEntity->assign<RigidBodyComponent>();
        rigidBody->isKinematic = true;

        nextWaveIn = waveInterval;

        GameState::Instance().money += 50;
    }
}
