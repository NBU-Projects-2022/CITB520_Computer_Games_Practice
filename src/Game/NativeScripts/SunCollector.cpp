#include "Scripts.h"

#include "ComponentSystem/Components.h"
#include <iostream>

void SunCollector::OnInit() {
	AddComponent<TransformComponent>(0, 0, DRAW_LAYER_10);
	Collider* sunCollectorBoxCollider = new BoxCollider(GetEntity(), -10, -10, 10, 10);
	sunCollectorBoxCollider->collisionLayer = CollisionLayers::LAYER_6;
	sunCollectorBoxCollider->collidesWithLayers = CollisionLayers::LAYER_6 | CollisionLayers::SUN;
	AddComponent<ColliderComponent>(Ref<Collider>(sunCollectorBoxCollider));
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
			}
		}
	}
}