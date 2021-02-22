#include "Scripts.h"

#include "ComponentSystem/Components.h"
#include <iostream>

void SunCollector::OnInit() {
	AddComponent<TransformComponent>(0, 0, DRAW_LAYER_10);
	Collider* sunCollectorCollider = new CircleCollider(GetEntity(), 20.0f);
	sunCollectorCollider->collidesWithLayers = CollisionLayers::LAYER_MASK | CollisionLayers::SUN;
	sunCollectorCollider->debugColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	AddComponent<ColliderComponent>(Ref<Collider>(sunCollectorCollider));
}

void SunCollector::Update(float deltaTime) {
	// Update with cursor position
	ImGuiIO& io = ImGui::GetIO();
	GetComponent<TransformComponent>().position.x = io.MousePos.x;
	GetComponent<TransformComponent>().position.y = SCREEN_HEIGHT - io.MousePos.y;

	// Check for collision with suns
	if (io.MouseClicked[0])
	{
		for (auto& collision : GetComponent<ColliderComponent>().collider->collisions)
		{
			auto& collider = collision.otherEntity->get<ColliderComponent>()->collider;
			if ((int)(collider->collisionLayer & CollisionLayers::SUN) > 0) {
				collision.otherEntity->get<NativeScriptComponent>()->Script<SunScript>()->Collect();
				// should we collect only one like in the real game?
			}
		}
	}
}