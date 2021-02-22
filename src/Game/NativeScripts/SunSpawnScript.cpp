#include <iostream>

#pragma warning(push, 0)
#include <SDL.h>
#include <imgui.h>
#pragma warning(pop)

#include "Scripts.h"
#include "ComponentSystem/Components.h"
#include "Core/AssetManager.h"
#include "Physics/Colliders.h"


void SunSpawnScript::OnInit()
{
    sun = AssetManager::LoadCachedImageFile("assets/sprites/Sun.png");
    glm::vec2 size(sun->GetWidth(), sun->GetHeight());
    sunSprite = Sprite(0, 0, size.x, size.y, sun.get(), -size / 2.0f);
}

void SunSpawnScript::Update(float deltaTime)
{
    nextSpawnIn -= deltaTime;
    if (nextSpawnIn < 0)
    {
        auto& position = GetComponent<TransformComponent>().position;
        auto sunEntity = CreateGameObject();

        sunEntity->assign<TransformComponent>(position);
        sunEntity->assign<RenderComponent>(CreateRef<Sprite>(sunSprite));

        Collider* sunCollider = new CircleCollider(sunEntity, sun->GetWidth() / 2.4f);
        // Collider* sunCollider = new BoxCollider(sunEntity, 0, 0, (float)sun->GetWidth(), (float)sun->GetHeight());
        sunCollider->collisionLayer = collisionLayer | CollisionLayers::SUN;
        sunCollider->collidesWithLayers = collisionLayer | CollisionLayers::GROUND;
        sunCollider->debugColor = glm::vec4(0.45f, 0.0f, 0.5f, 1.0f);
        sunEntity->assign<ColliderComponent>(Ref<Collider>(sunCollider));

        sunEntity->assign<NativeScriptComponent>()->Bind<SunScript>();

        auto rigidBody = sunEntity->assign<RigidBodyComponent>();
        rigidBody->velocity.y = rnd.Float(90.0f, 115.0f);
        rigidBody->velocity.x = rnd.Float(-20.0f, 20.0f);
        rigidBody->isKinematic = true;

        nextSpawnIn = spawnInterval;
    }
}
