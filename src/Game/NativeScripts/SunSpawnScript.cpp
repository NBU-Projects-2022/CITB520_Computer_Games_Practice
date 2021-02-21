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
    sunSprite = Sprite(0, 0, sun->GetWidth(), sun->GetHeight(), sun.get());
}

void SunSpawnScript::Update(float deltaTime)
{
    nextSpawnIn -= deltaTime;
    if (nextSpawnIn < 0)
    {
        GameState::Instance().money += 24;

        auto& position = GetComponent<TransformComponent>().position;
        auto sunEntity = CreateGameObject();

        sunEntity->assign<TransformComponent>(position);
        sunEntity->assign<RenderComponent>(CreateRef<Sprite>(sunSprite));
        //Collider* sunCircleCollider = new CircleCollider(sunEntity, (float)sun->GetWidth());
        //sunCircleCollider->collisionLayer = CollisionLayers::LAYER_6 | CollisionLayers::SUN;
        //sunEntity->assign<ColliderComponent>(Ref<Collider>(sunCircleCollider));

        Collider* sunBoxCollider = new BoxCollider(sunEntity, 0, 0, (float)sun->GetWidth(), (float)sun->GetHeight());
        sunBoxCollider->collisionLayer = CollisionLayers::LAYER_6 | CollisionLayers::SUN;
        sunEntity->assign<ColliderComponent>(Ref<Collider>(sunBoxCollider));

        sunEntity->assign<NativeScriptComponent>()->Bind<SunScript>();

        auto rigidBody = sunEntity->assign<RigidBodyComponent>();
        rigidBody->isKinematic = true;

        nextSpawnIn = spawnInterval;
    }
}
