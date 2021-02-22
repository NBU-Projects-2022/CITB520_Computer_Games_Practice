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
    glm::vec2 size(bullet->GetWidth(), bullet->GetHeight());
    bulletSprite = Sprite(0, 0, size.x, size.y, bullet.get(), -size / 2.0f);
}

void BulletSpawnScript::Update(float deltaTime)
{
    nextSpawnIn -= deltaTime;
    if (nextSpawnIn < 0)
    {
        auto& position = GetComponent<TransformComponent>().position;
        auto bulletEntity = CreateGameObject();

        bulletEntity->assign<TransformComponent>(position);
        bulletEntity->assign<RenderComponent>(CreateRef<Sprite>(bulletSprite));

        Collider* bulletCircleCollider = new CircleCollider(bulletEntity, bullet->GetWidth() / 2.0f);
        bulletCircleCollider->collisionLayer = collisionLayer | CollisionLayers::BULLET;
        bulletCircleCollider->collidesWithLayers = collisionLayer | CollisionLayers::ZOMBIE;
        bulletCircleCollider->debugColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

        bulletEntity->assign<ColliderComponent>(Ref<Collider>(bulletCircleCollider));
        bulletEntity->assign<NativeScriptComponent>()->Bind<BulletScript>();

        auto rigidBody = bulletEntity->assign<RigidBodyComponent>();
        rigidBody->isKinematic = true;

        nextSpawnIn = spawnInterval;
    }
}
