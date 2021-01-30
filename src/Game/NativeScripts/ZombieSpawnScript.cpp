#include "Scripts.h"

#include "ComponentSystem/Components.h"
#include "Core/AssetManager.h"
#include "Physics/Colliders.h"

void ZombieSpawnScript::OnInit() {
    zombie = AssetManager::LoadImageFile("assets/sprites/zombie.png");
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
        Collider * zombieBoxCollider = new BoxCollider(zombieEntity, 0, 0, zombie->GetWidth(), zombie->GetHeight());
        zombieBoxCollider->collisionLayer = CollisionLayers::ZOMBIE;
        zombieBoxCollider->collidesWithLayers = CollisionLayers::GROUND
            | CollisionLayers::PLANT
            | CollisionLayers::PROJECTILE;
        zombieEntity->assign<ColliderComponent>(Ref<Collider>(zombieBoxCollider));
        zombieEntity->assign<NativeScriptComponent>()->Bind<ZombieScript>();
        auto rigidBody = zombieEntity->assign<RigidBodyComponent>();
        rigidBody->isKinematic = true;

        nextWaveIn = waveInterval;
    }
}
