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
	int w, h;
	switch (plantType)
	{
		case (int)PlantTypes::Peashooter:
			plantEntity->assign<RenderComponent>(CreateRef<Sprite>(peashooterSprite));
			w = peashooter->GetWidth(), h = peashooter->GetHeight();
			// TODO: add shooter component
			break;
		case (int)PlantTypes::Sunflower:
			plantEntity->assign<RenderComponent>(CreateRef<Sprite>(sunflowerSprite));
			w = sunflower->GetWidth(), h = sunflower->GetHeight();
			break;
		case (int)PlantTypes::Wallnut:
			plantEntity->assign<RenderComponent>(CreateRef<Sprite>(wallnutSprite));
			w = wallnut->GetWidth(), h = wallnut->GetHeight();
			break;
		case (int)PlantTypes::Tallnut:
			plantEntity->assign<RenderComponent>(CreateRef<Sprite>(tallnutSprite));
			w = tallnut->GetWidth(), h = tallnut->GetHeight();
			break;
		default:
			// TODO handle this, we should not reach here
			break;
	}
	// Here we offsetthe sprite around the collision box.
	// We compare the sizes of the collision box (which has the same size as a single land plot) and the sprite.
	// Whichever is smaller gets centered within the bigger, with the exception where
	// if the sprite is taller than the collision height, then we want to keep the collision at the bottom of the sprite,
	// rather than centering it vertically.

	bool plotWidthIsLarger = false;
	bool plotHeightIsLarger = false;
	float xOffset = 0.0f;
	float yOffset = 0.0f;

	if (PLOT_W > w) {
		// center sprite within collision horizontally
		plotWidthIsLarger = true;
		xOffset = (PLOT_W - w) / 2;
	}
	else if (w > PLOT_W) {
		// center collision within sprite horizontally
		xOffset = (w - PLOT_W) / 2;
	}
	if (PLOT_H > h) {
		// center sprite within collision vertically
		plotHeightIsLarger = true;
		yOffset = (PLOT_H - h) / 2;
	}
	else if (h > PLOT_H) {
		// send collision to the bottom of the sprite
		yOffset = h - PLOT_H;
	}

	// offset is inverted if a plot dimension is smaller than the respective sprite dimension
	if (!plotWidthIsLarger && xOffset != 0.0f) xOffset *= -1;
	if (!plotHeightIsLarger && yOffset != 0.0f) yOffset *= -1;

	plantEntity->assign<TransformComponent>(x + xOffset, 720 - y + yOffset, DRAW_LAYER_10);

	Collider* plantBoxCollider = new BoxCollider(plantEntity, PLOT_W / 2, PLOT_H / 2, 1, 1);
	plantBoxCollider->collisionLayer = CollisionLayers::LAYER_MASK | CollisionLayers::PLANT;
	plantBoxCollider->collidesWithLayers = CollisionLayers::LAYER_MASK 
										 | CollisionLayers::GROUND
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