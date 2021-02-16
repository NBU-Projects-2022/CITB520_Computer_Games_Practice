#include <iostream>

#pragma warning(push, 0)
#include <SDL.h>
#include <imgui.h>
#pragma warning(pop)

#include "Scripts.h"
#include "ComponentSystem/Components.h"
#include "Core/AssetManager.h"
#include "Physics/Colliders.h"


void BulletSpawnScript::OnInit()
{
    bullet = AssetManager::LoadCachedImageFile("assets/sprites/Bullet.png");
    bulletSprite = Sprite(0, 0, bullet->GetWidth(), bullet->GetHeight(), bullet.get());
}

void BulletSpawnScript::Update(float deltaTime)
{
    nextSpawnIn -= deltaTime;
    if (nextSpawnIn < 0) {
        auto& position = GetComponent<TransformComponent>().position;
        auto bulletEntity = CreateGameObject();
        bulletEntity->assign<TransformComponent>(position); // assign() takes arguments and passes them to the constructor
        bulletEntity->assign<RenderComponent>(CreateRef<Sprite>(bulletSprite));
        Collider* bulletCircleCollider = new CircleCollider(bulletEntity, (float) bullet->GetWidth());
        bulletCircleCollider->collisionLayer = CollisionLayers::PLANT;
        bulletCircleCollider->collidesWithLayers = CollisionLayers::GROUND
            | CollisionLayers::PLANT
            | CollisionLayers::PROJECTILE;
        bulletEntity->assign<ColliderComponent>(Ref<Collider>(bulletCircleCollider));
        bulletEntity->assign<NativeScriptComponent>()->Bind<BulletScript>();
        auto rigidBody = bulletEntity->assign<RigidBodyComponent>();
        rigidBody->isKinematic = true;

        nextSpawnIn = spawnInterval;
    }
}
