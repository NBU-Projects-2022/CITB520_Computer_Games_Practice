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

#include <iostream>

int PlantSpawnScript::plantType = (int)PlantTypes::None;
bool PlantSpawnScript::shouldSpawn = false;
bool PlantSpawnScript::isHoldingPlant = false;

void PlantSpawnScript::OnInit()
{
}

void PlantSpawnScript::OnSpawnPress()
{
	int x, y;
	SDL_GetMouseState(&x, &y);

	auto plantEntity = CreateGameObject();

	plantEntity->assign<RenderComponent>(CreateRef<Sprite>(GameState::Instance().plantStates.GetSpriteByType(plantType)));
	plantEntity->assign<TransformComponent>(x, 720 - y, DRAW_LAYER_10);

	float xOffset = PLOT_W / 2;
	float yOffset = PLOT_H / 2;
	Collider* plantBoxCollider = new BoxCollider(plantEntity, xOffset, yOffset, xOffset + 1, yOffset + 1);
	plantBoxCollider->debugColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	plantBoxCollider->collisionLayer = CollisionLayers::LAYER_MASK;
	plantBoxCollider->collidesWithLayers = CollisionLayers::LAYER_MASK
										 | CollisionLayers::GROUND
										 | CollisionLayers::PLANT;
	plantEntity->assign<ColliderComponent>(Ref<Collider>(plantBoxCollider));
	plantEntity->assign<NativeScriptComponent>()->Bind<PlantScript>();
	auto rigidBody = plantEntity->assign<RigidBodyComponent>();
	rigidBody->isKinematic = true;
}

void PlantSpawnScript::Update(float deltaTime)
{
	if (shouldSpawn && !isHoldingPlant)
	{
		shouldSpawn = false;
		isHoldingPlant = true;
		OnSpawnPress();
	}
}