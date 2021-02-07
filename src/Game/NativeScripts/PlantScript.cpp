#include "Scripts.h"

#include "ComponentSystem/Components.h"
#include "Core/AssetManager.h"
#include "Physics/Colliders.h"
#include "../Game.h"
#include <iostream>

void PlantScript::OnInit()
{
    std::cout << "plant " << GetComponent<TransformComponent>().position.x << GetComponent<TransformComponent>().position.y << std::endl;
    // auto& position = GetComponent<TransformComponent>().position;
	/*auto plantEntity = CreateGameObject();
    plantEntity->assign<TransformComponent>((float)x, (float)y);
    std::cout << GetComponent<TransformComponent>().position.x << std::endl;
    plantEntity->assign<RenderComponent>(CreateRef<Sprite>(plantSprite));

    Collider* plantBoxCollider = new BoxCollider(plantEntity, 0, 0, plant->GetWidth(), plant->GetHeight());
    plantBoxCollider->collisionLayer = CollisionLayers::ZOMBIE;
    plantBoxCollider->collidesWithLayers = CollisionLayers::GROUND
        | CollisionLayers::PLANT
        | CollisionLayers::PROJECTILE;
    plantEntity->assign<ColliderComponent>(Ref<Collider>(plantBoxCollider));
    plantEntity->assign<NativeScriptComponent>()->Bind<PlantScript>();
    auto rigidBody = plantEntity->assign<RigidBodyComponent>();
    rigidBody->isKinematic = true;*/
}

void PlantScript::Update(float deltaTime) {

    switch (Game::event.type)
    {
    case SDL_MOUSEMOTION:
        //int x, y;
      //  SDL_GetRelativeMouseState(&x, &y);
       // GetComponent<TransformComponent>().position.x = x;
       // GetComponent<TransformComponent>().position.y = y;

    default:
        break;
    }
    //auto plantEntity = CreateGameObject();
   // plantEntity->assign<TransformComponent>((float)x, (float)y);
    //plantEntity->assign<RenderComponent>(CreateRef<Sprite>(plantSprite));

    /*Collider* plantBoxCollider = new BoxCollider(plantEntity, 0, 0, plant->GetWidth(), plant->GetHeight());
    plantBoxCollider->collisionLayer = CollisionLayers::ZOMBIE;
    plantBoxCollider->collidesWithLayers = CollisionLayers::GROUND
        | CollisionLayers::PLANT
        | CollisionLayers::PROJECTILE;
    plantEntity->assign<ColliderComponent>(Ref<Collider>(plantBoxCollider));
    plantEntity->assign<NativeScriptComponent>()->Bind<PlantScript>();
    auto rigidBody = plantEntity->assign<RigidBodyComponent>();
    rigidBody->isKinematic = true;*/
}
