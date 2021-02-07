#include "Game.h"

#include "ComponentSystem/Components.h"
#include "Game/NativeScripts/Scripts.h"
#include "Physics/Colliders.h"

//SDL_Event Game::event;
//ECS::World* Game::world;

void Game::Init() {
    auto plantSpawn = CreateGameObject();
    plantSpawn->assign<NativeScriptComponent>()->Bind<PlantSpawnScript>();

    auto zombieSpawn = CreateGameObject();
    zombieSpawn->assign<TransformComponent>(800.0f, 150.0f);
    zombieSpawn->assign<NativeScriptComponent>()->Bind<ZombieSpawnScript>();

    auto groundEntity = CreateGameObject();
    groundEntity->assign<TransformComponent>(0.0f, 0.0f); // assign() takes arguments and passes them to the constructor
    Collider * groundBoxCollider = new BoxCollider(groundEntity, 0, 0, 1920, 120);
    groundEntity->assign<ColliderComponent>(Ref<Collider>(groundBoxCollider));
    groundEntity->assign<RigidBodyComponent>(true);

    level.Init();
}

void Game::OnWindowResize(int newWidth, int newHeight) {
    level.OnWindowResize(newWidth, newHeight);
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
