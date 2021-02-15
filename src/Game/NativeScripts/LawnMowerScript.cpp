#pragma warning(push, 0)
#include <imgui.h>
#pragma warning(pop)

#include "Scripts.h"
#include "ComponentSystem/Components.h"
#include "Physics/Colliders.h"

// #include <iostream>

void LawnMowerScript::Update(float deltaTime) {
    const auto & colliderComponent = GetComponent<ColliderComponent>();
    if (!colliderComponent.collider->collisions.empty()) {
        auto & rigidBody = GetComponent<RigidBodyComponent>();
        rigidBody.velocity.x = -10.0;
    }
}
