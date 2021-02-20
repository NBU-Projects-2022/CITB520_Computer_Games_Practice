#include "Game.h"

#include "ComponentSystem/Components.h"
#include "Game/NativeScripts/Scripts.h"
#include "Physics/Colliders.h"
#include "Core/AssetManager.h"

//SDL_Event Game::event;
//ECS::World* Game::world;

void Game::Init() {
	// Set background
	auto backgroundEntity = CreateGameObject();
	backgroundEntity->assign<TransformComponent>(0.0f, 0.0f, DRAW_LAYER_0);
	texture = AssetManager::LoadCachedImageFile("assets/sprites/background1.jpg");
	sprite = CreateRef<Sprite>(0, 0, texture->GetWidth(), texture->GetHeight(), texture.get());
	backgroundEntity->assign<RenderComponent>(sprite);

	// Create a plant spawner
    auto plantSpawn = CreateGameObject();
    plantSpawn->assign<NativeScriptComponent>()->Bind<PlantSpawnScript>();

	// Create a wave controller
    auto waveController = CreateGameObject();
    waveController->assign<NativeScriptComponent>()->Bind<WaveControllerScript>();

	const int BOTTOM_ROW_Y = 30, ROW_SIZE = 100;
	for (int i = 0; i < ROWS; i++) {
		// Create a zombie spawner for each row
		auto zombieSpawn = CreateGameObject();
		zombieSpawn->assign<TransformComponent>(LAWN_X + COLS * PLOT_W, BOTTOM_ROW_Y + ROW_SIZE * i, DRAW_LAYER_9 - 0.1f * i);
		auto script = zombieSpawn->assign<NativeScriptComponent>()->Bind<ZombieSpawnScript>();
		script->SetCollisionLayer(i);

		for (int j = 0; j < COLS; j++) {
			auto groundEntity = CreateGameObject();
			//Ref<TextureGPU> ground_texture = AssetManager::LoadCachedImageFile("assets/sprites/Credits_BigBrain.jpg");
			//Ref<Sprite> ground_sprite = CreateRef<Sprite>(0, 0, ground_texture->GetWidth(), ground_texture->GetHeight(), ground_texture.get());

			groundEntity->assign<TransformComponent>(LAWN_X + j * PLOT_W, LAWN_Y + i * PLOT_H, DRAW_LAYER_9 - 0.1f * i);//, 0, (float) PLOT_W/ground_texture->GetWidth(), (float) PLOT_H/ground_texture->GetHeight());
			Collider* groundBoxCollider = new BoxCollider(groundEntity, 0, 0, PLOT_W, PLOT_H);
			groundEntity->assign<ColliderComponent>(Ref<Collider>(groundBoxCollider));
			groundEntity->assign<RigidBodyComponent>(true);
			//groundEntity->assign<RenderComponent>(ground_sprite);
		}
	}
}

void Game::OnWindowResize(int newWidth, int newHeight) {
    //level.OnWindowResize(newWidth, newHeight);
}

void Game::Update(float deltaTime) {
    auto & instance = GameState::Instance();
	instance.totalGameTime += deltaTime;
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
