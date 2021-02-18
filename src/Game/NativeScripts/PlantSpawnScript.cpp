#include <iostream>

#pragma warning(push, 0)
#include <SDL.h>
#include <imgui.h>
#pragma warning(pop)

#include "Scripts.h"
#include "ComponentSystem/Components.h"
#include "Core/AssetManager.h"
#include "Physics/Colliders.h"

int PlantSpawnScript::plantType = (int)PlantTypes::None;
bool PlantSpawnScript::shouldSpawn = false;

void PlantSpawnScript::OnInit()
{
	peashooter = AssetManager::LoadCachedImageFile("assets/sprites/Peashooter.png");
	sunflower = AssetManager::LoadCachedImageFile("assets/sprites/Sunflower.png");
	wallnut = AssetManager::LoadCachedImageFile("assets/sprites/Wallnut1.png");
	tallnut = AssetManager::LoadCachedImageFile("assets/sprites/Tallnut1.png");

	peashooterSprite = Sprite(0, 0, peashooter->GetWidth(), peashooter->GetHeight(), peashooter.get());
	sunflowerSprite = Sprite(0, 0, sunflower->GetWidth(), sunflower->GetHeight(), sunflower.get());
	wallnutSprite = Sprite(0, 0, wallnut->GetWidth(), wallnut->GetHeight(), wallnut.get());
	tallnutSprite = Sprite(0, 0, tallnut->GetWidth(), tallnut->GetHeight(), tallnut.get());

}

void PlantSpawnScript::OnSpawnPress()
{
	int x, y;
	SDL_GetMouseState(&x, &y);

	auto plantEntity = CreateGameObject();
	plantEntity->assign<TransformComponent>(x, 720 - y, DRAW_LAYER_10);
	Collider* plantBoxCollider = nullptr;
	switch (plantType)
	{
		case (int)PlantTypes::Peashooter:
			plantEntity->assign<RenderComponent>(CreateRef<Sprite>(peashooterSprite));
			plantBoxCollider = new BoxCollider(plantEntity, 0, 0, (float) peashooter->GetWidth(), (float) peashooter->GetHeight());
			//add shooter component
			break;
		case (int)PlantTypes::Sunflower:
			plantEntity->assign<RenderComponent>(CreateRef<Sprite>(sunflowerSprite));
			plantBoxCollider = new BoxCollider(plantEntity, 0, 0, (float) sunflower->GetWidth(), (float) sunflower->GetHeight());
			break;
		case (int)PlantTypes::Wallnut:
			plantEntity->assign<RenderComponent>(CreateRef<Sprite>(wallnutSprite));
			plantBoxCollider = new BoxCollider(plantEntity, 0, 0, (float) wallnut->GetWidth(), (float) wallnut->GetHeight());
			break;
		case (int)PlantTypes::Tallnut:
			plantEntity->assign<RenderComponent>(CreateRef<Sprite>(tallnutSprite));
			plantBoxCollider = new BoxCollider(plantEntity, 0, 0, (float) tallnut->GetWidth(), (float) tallnut->GetHeight());
			break;
		default:
			break;
	}

	plantBoxCollider->collisionLayer = CollisionLayers::PLANT;
	plantBoxCollider->collidesWithLayers = CollisionLayers::GROUND
		| CollisionLayers::ZOMBIE
		| CollisionLayers::PROJECTILE;
	plantEntity->assign<ColliderComponent>(Ref<Collider>(plantBoxCollider));
	plantEntity->assign<NativeScriptComponent>()->Bind<PlantScript>();
	auto rigidBody = plantEntity->assign<RigidBodyComponent>();
	rigidBody->isKinematic = true;

	shouldSpawn = false;
}

void PlantSpawnScript::Update(float deltaTime)
{
	if (shouldSpawn)
	{
		OnSpawnPress();
	}
}