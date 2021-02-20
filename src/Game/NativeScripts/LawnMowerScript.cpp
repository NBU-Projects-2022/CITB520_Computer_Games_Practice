#pragma warning(push, 0)
#include <imgui.h>
#pragma warning(pop)

#include "Scripts.h"
#include "ComponentSystem/Components.h"
#include "Physics/Colliders.h"

// #include <iostream>

void LawnMowerScript::OnInit()
{
    lawnMower = AssetManager::LoadCachedImageFile("assets/sprites/lawnmower.png");
    lawnMowerSprite = Sprite(0, 0, lawnMower->GetWidth(), lawnMower->GetHeight(), lawnMower.get());

	entity->assign<RenderComponent>(CreateRef<Sprite>(lawnMowerSprite));

	Collider* lawnMowerBoxCollider = new BoxCollider(entity, 0, 0, lawnMower->GetWidth(), PLOT_H);
	lawnMowerBoxCollider->debugColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	lawnMowerBoxCollider->collisionLayer = collisionLayer | CollisionLayers::LAWNMOWER;
	lawnMowerBoxCollider->collidesWithLayers = collisionLayer | CollisionLayers::ZOMBIE;
	entity->assign<ColliderComponent>(Ref<Collider>(lawnMowerBoxCollider));

	auto rigidBody = entity->assign<RigidBodyComponent>();
	rigidBody->isKinematic = true;

	auto& position = GetComponent<TransformComponent>().position;
	position.x -= lawnMower->GetWidth();
}

void LawnMowerScript::Update(float deltaTime) {
	
	auto& position = GetComponent<TransformComponent>().position;

	if (position.x > SCREEN_WIDTH)
	{
		shouldDestroy = true;
		return;
	}
	
	// Check if the lawn mower has already started
	auto& rigidBody = GetComponent<RigidBodyComponent>();
	if (rigidBody.velocity.x != 0.f)
	{
		return;
	}

	const auto & colliderComponent = GetComponent<ColliderComponent>();
	for (auto& collision : GetComponent<ColliderComponent>().collider->collisions)
	{
		if ((int)(collision.otherEntity->get<ColliderComponent>()->collider->collisionLayer & CollisionLayers::ZOMBIE) > 0)
		{
			rigidBody.velocity.x = speed;
		}
	}
}
