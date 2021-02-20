#include <iostream>

#pragma warning(push, 0)
#include <imgui.h>
#pragma warning(pop)

#include "Scripts.h"
#include "ComponentSystem/Components.h"


void ZombieScript::Update(float deltaTime) {
    auto & rigidBody = GetComponent<RigidBodyComponent>();

    bool collidesWithPlant = false;
    for (auto& collision : GetComponent<ColliderComponent>().collider->collisions)
    {
        //check if this works
        if ((int)(collision.otherEntity->get<ColliderComponent>()->collider->collisionLayer & CollisionLayers::PLANT) > 0)
        {
            collidesWithPlant = true;
            rigidBody.velocity.x = 0;
        }
    }

    if (!collidesWithPlant)
    {
        rigidBody.velocity.x = -zombieSpeed;

    }
    // ImGuiIO& io = ImGui::GetIO();
    // if (rigidBody.isKinematic) {
    //     rigidBody.isKinematic = false;
    // } else if (ImGui::IsKeyReleased(ImGui::GetKeyIndex(ImGuiKey_Space))) {
    //     glm::vec2 & velocity = rigidBody.velocity;
    //     glm::vec2 & acceleration = rigidBody.acceleration;
    //     if (glm::abs(velocity.y) < 0.1) {
    //         acceleration.y = 0;
    //         velocity.y = 150;
    //         rigidBody.isKinematic = true;
    //     }
    // }
}
