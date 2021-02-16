#include "Game.h"

#include "ComponentSystem/Components.h"
#include "Game/NativeScripts/Scripts.h"
#include "Physics/Colliders.h"
#include "Core/AssetManager.h"

//SDL_Event Game::event;
//ECS::World* Game::world;

void Game::Init() {
	auto backgroundEntity = CreateGameObject();
	backgroundEntity->assign<TransformComponent>(0.0f, 0.0f, DRAW_LAYER_0);
	texture = AssetManager::LoadCachedImageFile("assets/sprites/background1.jpg");
	sprite = CreateRef<Sprite>(0, 0, texture->GetWidth(), texture->GetHeight(), texture.get());
	backgroundEntity->assign<RenderComponent>(sprite);

    auto plantSpawn = CreateGameObject();
    plantSpawn->assign<NativeScriptComponent>()->Bind<PlantSpawnScript>();

	const int BOTTOM_ROW_Y = 30, ROW_SIZE = 100;
	for (int i = 0; i < 5; i++) {
		auto zombieSpawn = CreateGameObject();
		zombieSpawn->assign<TransformComponent>(800.0f, BOTTOM_ROW_Y + ROW_SIZE * i, DRAW_LAYER_9 - 0.1f * i);
		zombieSpawn->assign<NativeScriptComponent>()->Bind<ZombieSpawnScript>();

		auto groundEntity = CreateGameObject();
		groundEntity->assign<TransformComponent>(0.0f, BOTTOM_ROW_Y + ROW_SIZE * i, DRAW_LAYER_9 - 0.1f * i);
		Collider* groundBoxCollider = new BoxCollider(groundEntity, 0, 0, 1920, 120);
		groundEntity->assign<ColliderComponent>(Ref<Collider>(groundBoxCollider));
		groundEntity->assign<RigidBodyComponent>(true);

		for (int j = 0; j < 8; j++) {
			// ...
		}
	}
}

void Game::OnWindowResize(int newWidth, int newHeight) {
    //level.OnWindowResize(newWidth, newHeight);
}

void Game::Update(float deltaTime) {
    // level.Update(deltaTime);
}

void Game::DestroyGameObject(ECS::Entity* ent, bool immediate)
{
	if (ent == nullptr)
		return;

	world->destroy(ent, immediate);
	/*if (ent->isPendingDestroy())
	{
		if (immediate)
		{
			entities.erase(std::remove(entities.begin(), entities.end(), ent), entities.end());
			std::allocator_traits<EntityAllocator>::destroy(entAlloc, ent);
			std::allocator_traits<EntityAllocator>::deallocate(entAlloc, ent, 1);
		}

		return;
	}

	ent->bPendingDestroy = true;

	emit<Events::OnEntityDestroyed>({ ent });

	if (immediate)
	{
		entities.erase(std::remove(entities.begin(), entities.end(), ent), entities.end());
		std::allocator_traits<EntityAllocator>::destroy(entAlloc, ent);
		std::allocator_traits<EntityAllocator>::deallocate(entAlloc, ent, 1);
	}*/
}
