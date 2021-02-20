#include <iostream>

#pragma warning(push, 0)
#include <SDL.h>
#include <imgui.h>
#pragma warning(pop)

#include "Scripts.h"
#include "ComponentSystem/Components.h"
#include "Core/AssetManager.h"
#include "Physics/Colliders.h"
#include "Game/Game.h"

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
	switch (plantType)
	{
		case (int)PlantTypes::Peashooter:
			plantEntity->assign<RenderComponent>(CreateRef<Sprite>(peashooterSprite));
			// TODO: add shooter component
			break;
		case (int)PlantTypes::Sunflower:
			plantEntity->assign<RenderComponent>(CreateRef<Sprite>(sunflowerSprite));
			break;
		case (int)PlantTypes::Wallnut:
			plantEntity->assign<RenderComponent>(CreateRef<Sprite>(wallnutSprite));
			break;
		case (int)PlantTypes::Tallnut:
			plantEntity->assign<RenderComponent>(CreateRef<Sprite>(tallnutSprite));
			break;
		default:
			// TODO handle this, we should not reach here
			break;
	}

	plantEntity->assign<TransformComponent>(x, 720 - y, DRAW_LAYER_10);

	float xOffset = PLOT_W / 2;
	float yOffset = PLOT_H / 2;
	Collider* plantBoxCollider = new BoxCollider(plantEntity, xOffset, yOffset, xOffset + 1, yOffset + 1);
	plantBoxCollider->debugColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	plantBoxCollider->collisionLayer = CollisionLayers::LAYER_MASK | CollisionLayers::PLANT;
	plantBoxCollider->collidesWithLayers = CollisionLayers::LAYER_MASK
										 | CollisionLayers::GROUND
										 | CollisionLayers::PLANT;
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