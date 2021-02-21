#include "Scripts.h"

#include "ComponentSystem/Components.h"
#include <iostream>


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
				reinterpret_cast<SunScript*>(collision.otherEntity->get<NativeScriptComponent>()->nativeScript.get())->Collect();
			}
		}
	}
}