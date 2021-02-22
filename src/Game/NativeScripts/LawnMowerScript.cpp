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

	AddComponent<RenderComponent>(CreateRef<Sprite>(lawnMowerSprite));

	Collider* lawnMowerBoxCollider = new BoxCollider(GetEntity(), 0, 0, lawnMower->GetWidth(), PLOT_H);
	lawnMowerBoxCollider->debugColor = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
	lawnMowerBoxCollider->collisionLayer = collisionLayer | CollisionLayers::LAWNMOWER;
	lawnMowerBoxCollider->collidesWithLayers = collisionLayer | CollisionLayers::ZOMBIE;
	AddComponent<ColliderComponent>(Ref<Collider>(lawnMowerBoxCollider));

	auto & rigidBody = AddComponent<RigidBodyComponent>();
	rigidBody.isKinematic = true;

	auto& position = GetComponent<TransformComponent>().position;
	position.x -= lawnMower->GetWidth();
}

void LawnMowerScript::Update(float deltaTime) {
	auto& position = GetComponent<TransformComponent>().position;

	if (position.x > SCREEN_WIDTH) {
		shouldDestroy = true;
		return;
	}

	// Check if the lawn mower has already started
	const auto & colliderComponent = GetComponent<ColliderComponent>();
	if (active) {
		for (auto& collision : GetComponent<ColliderComponent>().collider->collisions) {
			// this should always be a zombie?
			if ((int)(collision.otherEntity->get<ColliderComponent>()->collider->collisionLayer & CollisionLayers::ZOMBIE) > 0) {
				// using hp so we can display death animations (if we have any)
				collision.otherEntity->get<NativeScriptComponent>()->Script<NativeScript>()->hp = 0;
			}
		}

		return;
	}

	auto& rigidBody = GetComponent<RigidBodyComponent>();
	for (auto& collision : GetComponent<ColliderComponent>().collider->collisions) {
		// this should always be a zombie?
		if ((int)(collision.otherEntity->get<ColliderComponent>()->collider->collisionLayer & CollisionLayers::ZOMBIE) > 0) {
			Activate();
			rigidBody.velocity.x = speed;
			break;
		}
	}
}
