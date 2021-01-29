#include "Scripts.h"

#include <imgui.h>
#include <ComponentSystem/Components.h>

// #include <iostream>

void VehicleBehaviourScript::Update(ECS::Entity * entity, float deltaTime) {
    ImGuiIO& io = ImGui::GetIO();

    const auto & rigidBody = entity->get<RigidBodyComponent>();
    if (rigidBody->isKinematic) {
        rigidBody->isKinematic = false;
    } else if (ImGui::IsKeyReleased(ImGui::GetKeyIndex(ImGuiKey_Space))) {
        // std::cout << " space " << std::endl;
        glm::vec2 & velocity = rigidBody->velocity;
        glm::vec2 & acceleration = rigidBody->acceleration;
        if (glm::abs(velocity.y) < 0.1) {
            // std::cout << " jump " << std::endl;
            acceleration.y = 0;
            velocity.y = 150;
            rigidBody->isKinematic = true;
        }
    }
}
